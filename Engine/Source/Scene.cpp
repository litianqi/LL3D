#include "Scene.h"
#include "Core\Assert.h"
#include "Core\Exceptions.h"
#include "Graphics\Device.h"
#include "Graphics\Effects.h"
#include "Graphics\ModelRender.h"
#include "Graphics\Camera.h"
#include "Graphics\CommonStates.h"
#include "RecursiveSceneIterator.h"
#include "Graphics\LightComponent.h"
#include "Transform.h"

using namespace std::placeholders;
using namespace LL3D;

namespace {

//>
// Shadow stenciling appears before mirror stenciling.
// Shadow use 0 as no shadow rendered, and 1 as shadow renderd
// Mirror use 2 as mirror's scope (hence reflection can be renderd).
//
const auto ShadowStencilRef = 0u;
const auto MirrorStencilRef = 2u;

//>
// # Why ReflectionOffset and ShadowOffset? 
// When two pixel have same depth, DirectX is random at which one to use, even 
// if you render one of them latter.
// We render models in this order: opaque -> shadow -> mark mirror -> reflection. 
// And after mark mirror, though pixels of shadow intersected with mirror will be
// replaced, depth buffer will stay unchanged. 
// There is a possibilty that opaque, shadow and reflection have a same position.
// And hence these two offsets.
//
const auto ShadowOffset = Math::Vector3(0.01f, 0.01f, 0.01f);
const auto ReflectionOffset = Math::Vector3(0.02f, 0.02f, 0.02f);

bool SortBasedOnDistanceToCamera(Math::Vector3& camera_pos, 
  const RenderableMesh& lhs, const RenderableMesh& rhs)
{
  auto lhs_pos = lhs.first.position();
  auto rhs_pos = rhs.first.position();
  auto lhs_d = Math::Vector3::Distance(camera_pos, lhs_pos);
  auto rhs_d = Math::Vector3::Distance(camera_pos, rhs_pos);
  if (lhs_d != rhs_d)
    return lhs_d < rhs_d;
  return false;
};

//>
// Trival planar shadow rendering function. Shadow will be rendered at
// XZ plannar. (Note: This function is quit limited. TODO: improve this.)
//
void RenderPlanarShadow(Transform transform, 
  const Graphics::MeshRender& mesh, const std::vector<const GameObject*>& lights)
{
  const auto xz = Math::Plane(Math::Vector3(0.f, 0.f, 0.f), Math::Vector3::Up);
  auto mat = Graphics::Material();
  mat.diffuse = Math::Vector3(0.2f);
  mat.emissive = Math::Vector3::One;
  mat.transparent = Math::Vector3::One;
  mat.shininess = 5.f;
  mat.opacity = 0.5f;
  mat.shininess_strength = 1.f;
  mat.shadow = true;

  Math::Vector4 vec;

  for (const auto& light : lights) {
    const auto* light_component = light->component<Graphics::LightComponent>();
    switch (light_component->lightType()) {
    case Graphics::LightComponent::Ambient: {
      continue;
    }
    case Graphics::LightComponent::Directional: {
      const auto dir = -light->transform().direction();
      vec = Math::Vector4(dir.x, dir.y, dir.z, 0.f);
      break;
    }
    case Graphics::LightComponent::Point:
    case Graphics::LightComponent::Spot: {
      const auto pos = light->transform().position();
      vec = Math::Vector4(pos.x, pos.y, pos.z, 1.f);
      continue;
    }
    default: {
      ASSERT(!"Invalid light type, This is not a valid LightComponent");
    }
    }

    const auto shadow = Math::Matrix::CreateShadow(vec, xz);
    const auto& world = transform.matrix();
    Transform::render(world * shadow * Math::Matrix::CreateTranslation(ShadowOffset));
    const auto old_mat = mesh.material();
    // We lied, but we will recover it latter, it's a good lie.
    const_cast<Graphics::MeshRender&>(mesh).setMaterial(mat);
    mesh.render();
    // Recover old material like I said.
    const_cast<Graphics::MeshRender&>(mesh).setMaterial(old_mat);
  }
}

}  // namespace

namespace LL3D {

void Scene::add(std::unique_ptr<GameObject> object) {
  object->setScene(this);
  objects_.push_back(std::move(object));
}

void LL3D::Scene::update() {

  if (first_update_) {
    for (auto& object : objects_) {
      object->start();
    }
    first_update_ = false;
  }

  // Set per frame constant buffer.
  // Find first active camera.
  /*auto it = std::find_if(objects_.begin(), objects_.end(), [](auto& o) {
    
  })*/
  // TODO
  //const auto& camera = dynamic_cast<Graphics::Camera* const>((*((*objects_.begin()).GetComponents().begin())).get());
  //s_effect->SetEyePosW(camera->GetPosition());
  //s_effect->SetView(camera->GetViewMatrix());
  //s_effect->SetProjection(camera->GetFrustum().GetProjectionMaxtrix());

  for (auto& object : objects_) {
    object->update();
  }
}

//>
// For a detailed description of Render Procedure, reference to online doc:
// https://onedrive.live.com/redir?page=view&resid=CD6518D498235073!2141&authkey=!AMsU_BK42yKOQEU&wd=target%28Design.one%7C026CF69E-4746-4048-AE49-7E0AEFD55322%2FRender%20Procedure%7C7AB7AF3B-7E41-435E-91FB-99E5CEDC60DC%2F%29
//
void Scene::render() noexcept
{
  s_graphicsDevice->deviceContex()->ClearRenderTargetView(
    s_graphicsDevice->renderTargetView(),
    reinterpret_cast<const float*>(&Math::Color::Grey)
    );
  s_graphicsDevice->deviceContex()->ClearDepthStencilView(
    s_graphicsDevice->depthStencilView(),
    D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0, 0
    );

  s_graphicsDevice->deviceContex()->IASetInputLayout(s_inputLayout.Get());
  s_graphicsDevice->deviceContex()->IASetPrimitiveTopology(
    D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
    );

  // 0. Apply camera.

 const auto* _camera = camera();
  if (!_camera)
    return;
  _camera->transform().render();
  _camera->component<Graphics::Camera>()->render();

  // 1. Apply lights.
  auto _lights = lights();
  for (const auto* light : _lights) {
    light->transform().render();
    light->component<Graphics::LightComponent>()->render();
  }

  // 2. Render all opaque.

  // a. Render model itself.
  s_graphicsDevice->deviceContex()->OMSetBlendState(
    nullptr, nullptr, 0xffffffff
    );

  for (const auto& object : RecursiveSceneIterator(*this)) {
    const auto* model = object.component<Graphics::ModelRender>();
    if (!model)
      continue;
    const auto& transform = object.transform();
    transform.render();
    for (const auto& mesh : model->meshRenders())
    {
      if (mesh.opaque()) {
        mesh.render();
      }
    }
  }

  // b. Render model's shadow with blending and stenciling.
  s_graphicsDevice->deviceContex()->OMSetDepthStencilState(
    Graphics::CommonStates::shadow(), ShadowStencilRef);
  s_graphicsDevice->deviceContex()->OMSetBlendState(
    Graphics::CommonStates::multiply(), nullptr, 0xffffffff
    );

  for (const auto& object : RecursiveSceneIterator(*this)) {
    // Prevent earth have shadow (TODO: improve this)
    if (object.name() == "Earth")  
      continue;

    const auto* model = object.component<Graphics::ModelRender>();
    if (!model)
      continue;

    const auto& transform = object.transform();
    for (const auto& mesh : model->meshRenders())
    {
      if (mesh.opaque()) {
        RenderPlanarShadow(transform, mesh, _lights);
      }
    }
  }

  // 3. For each mirror:

  for (const auto& mirror : mirrors()) {
    // a. Mark mirror.
    s_graphicsDevice->deviceContex()->OMSetBlendState(
      nullptr, nullptr, 0xffffffff
      );
    s_graphicsDevice->deviceContex()->OMSetDepthStencilState(
      Graphics::CommonStates::markMirror(), MirrorStencilRef
      );
    mirror.first.render();
    mirror.second->render();

    // b. Reverse all lights. (TODO)
    const auto plane = Math::Plane(mirror.first.position(), mirror.first.direction());
    auto reflect = Math::XMMatrixReflect(plane);
    reflect *= Math::Matrix::CreateTranslation(ReflectionOffset);

    /*for (auto light : light) {
    auto light_copy = *light;
    auto transform = light_copy.GetComponent<Transform>();
    auto backup = transform->GetMatrix();
    transform->SetMatrix(backup * reflect);
    light_copy.GetComponent<Graphics::LightComponent>()->Update();
    }*/

    // c. Reverse all opaque and mirrors (except current one).
    // d. Render all opaque and mirrors (except current one) with stenciling 
    //    and without blending. 
    s_graphicsDevice->deviceContex()->RSSetState(  // Reverse changes winding 
      Graphics::CommonStates::cullClockwise());        // order, so cull clockwise.
    s_graphicsDevice->deviceContex()->OMSetDepthStencilState(
      Graphics::CommonStates::renderReflection(), MirrorStencilRef);

    for (const auto& object : RecursiveSceneIterator(*this)) {
      const auto* model = object.component<Graphics::ModelRender>();
      if (!model)
        continue;
      Transform::render(object.transform().matrix() * reflect);
      for (const auto& mesh : model->meshRenders())
      {
        if (mesh.opaque() || (mesh.mirror() && &mesh != mirror.second))
          mesh.render();
      }
    }

    // e. Reverse all transparents.
    // f. Sort reversed transparent based on distance to active Camera.
    auto _transparents = transparents();
    for (auto& transparent : _transparents) {
      transparent.first.setMatrix(transparent.first.matrix() * reflect);
    }
    const auto camera_pos = _camera->transform().position();
    std::sort(_transparents.begin(), _transparents.end(),
      std::bind(SortBasedOnDistanceToCamera, camera_pos, _1, _2));

    // g. Render sorted transparent with stenciling and blending.
    s_graphicsDevice->deviceContex()->OMSetBlendState(
      Graphics::CommonStates::alphaBlend(), nullptr, 0xffffffff
      );
    for (const auto& transparent : _transparents) {
      transparent.first.render();
      transparent.second->render();
    }

    // h. Restore lights. (TODO)
    /*for (auto light : lights) {
    light->GetComponent<Graphics::LightComponent>()->Update();
    }*/

    // i. Render current mirror with blending.
    s_graphicsDevice->deviceContex()->RSSetState(nullptr);
    s_graphicsDevice->deviceContex()->OMSetDepthStencilState(
      nullptr, 1);

    mirror.first.render();
    mirror.second->render();
  }

  // 4. Sort transparent.

  auto _transparents = transparents();
  const auto camera_pos = _camera->transform().position();
  std::sort(_transparents.begin(), _transparents.end(),
    std::bind(SortBasedOnDistanceToCamera, camera_pos, _1, _2));

  // 5. Render transparent with blending.

  for (const auto& transparent : _transparents) {
    transparent.first.render();
    transparent.second->render();
  }

  throwIfFailed(s_graphicsDevice->swapChain()->Present(0, 0));
}

GameObject * Scene::camera() noexcept
{
  for(auto& object : objects_) {
    auto camera = object->component<Graphics::Camera>();
    if (camera)
      return object.get();
  }
  return nullptr;
}

std::vector<const GameObject*> Scene::lights() noexcept
{
  auto result = std::vector<const GameObject*>();

  for (const auto& object : RecursiveSceneIterator(*this)) {
    const auto* light = object.component<Graphics::LightComponent>();
    if (light) {
      result.push_back(&object);
    }
  }

  return result;
}

std::vector<RenderableMesh> Scene::mirrors() noexcept
{
  auto result = std::vector<RenderableMesh>();

  for (const auto& object : RecursiveSceneIterator(*this)) {
    auto model = object.component<Graphics::ModelRender>();
    if (!model)
      continue;
    const auto& transform = object.transform();
    for (const auto& mesh : model->meshRenders())
    {
      if (mesh.mirror())
        result.push_back(RenderableMesh(transform, &mesh));
    }
  }

  return result;
}

std::vector<RenderableMesh> Scene::transparents() noexcept
{
  auto result = std::vector<RenderableMesh>();

  for (const auto& object : RecursiveSceneIterator(*this)) {
    const auto* model = object.component<Graphics::ModelRender>();
    if (!model)
      continue;
    const auto& transform = object.transform();
    for (const auto& mesh : model->meshRenders())
    {
      if (mesh.transparent())
        result.push_back(RenderableMesh(transform, &mesh));
    }
  }

  return result;
}

}  // namespace LL3D
