#include "Scene.h"
#include <limits>
#include "Core\Assert.h"
#include "Core\Exceptions.h"
#include "Graphics\Device.h"
#include "Graphics\Effects.h"
#include "Graphics\ModelRender.h"
#include "Graphics\Camera.h"
#include "Graphics\CommonStates.h"
#include "Graphics\LightComponent.h"
#include "Input\Mouse.h"
#include "RecursiveSceneIterator.h"
#include "Transform.h"
#include "Window.h"

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
      const auto dir = -light->transform().upVec();
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
    Transform::writeToEffect(world * shadow * Math::Matrix::CreateTranslation(ShadowOffset));
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
Scene::Scene(Window * window) :
  window_(window)
{
}

void Scene::add(std::unique_ptr<GameObject> object) {
  object->setScene(this);
  objects_.push_back(std::move(object));
}

const GameObject * Scene::picking() const
{
  return picking_;
}

void LL3D::Scene::update() {

  if (first_update_) {
    for (auto& object : objects_) {
      object->start();
    }
    first_update_ = false;
  }
  retrieveCamera();
  calculatePicking();
  for (auto& object : objects_) {
    object->update();
  }
}

//>
// For a detailed description of Render Procedure, reference to online doc:
// https://onedrive.live.com/redir?page=view&resid=CD6518D498235073!2141&authkey=!AMsU_BK42yKOQEU&wd=target%28Design.one%7C026CF69E-4746-4048-AE49-7E0AEFD55322%2FRender%20Procedure%7C7AB7AF3B-7E41-435E-91FB-99E5CEDC60DC%2F%29
//
void Scene::render()
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

  // Make sure pixels far away that depth values equals 1 (NDC) will be rendered.
  // (e.g. skybox)
  s_graphicsDevice->deviceContex()->OMSetDepthStencilState(
    Graphics::CommonStates::depthLessEqual(), 0 /*do not matter*/);
  s_graphicsDevice->deviceContex()->RSSetState(
    Graphics::CommonStates::cullNone());  // todo: remove

  // 0. Apply camera.

 const auto* _camera = camera();
  if (!_camera)
    return;
  _camera->transform().writeToEffect();
  _camera->component<Graphics::Camera>()->writeToEffect();

  // 1. Apply lights.
  auto _lights = lights();
  for (const auto* light : _lights) {
    light->transform().writeToEffect();
    light->component<Graphics::LightComponent>()->writeToEffect();
  }

  // 2. Render all opaque.

  // a. Render model itself.
  s_graphicsDevice->deviceContex()->OMSetBlendState(
    nullptr, nullptr, 0xffffffff
    );
  auto frustum = _camera->component<Graphics::Camera>()->frustum();

  for (const auto& object : RecursiveSceneIterator(*this)) 
  {
    const auto* model = object.component<Graphics::ModelRender>();
    if (model) 
    {
      const auto localBox = model->localBoundingBox();
      const auto& transform = object.transform();
      const auto worldBox = convertToWorldBoundingBox(localBox, transform);
      if (frustum.Intersects(worldBox)) 
      {
        const auto& transform = object.transform();
        transform.writeToEffect();
        for (const auto& mesh : *model)
        {
          if (mesh.opaque()) 
          {
            mesh.render();
          }
        }
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
    const auto* model = object.component<Graphics::ModelRender>();
    if (model &&
      object.name() != "Earth") 
    {
      const auto localBox = model->localBoundingBox();
      const auto& transform = object.transform();
      const auto worldBox = convertToWorldBoundingBox(localBox, transform);
      if (frustum.Intersects(worldBox))
      {
        const auto& transform = object.transform();
        for (const auto& mesh : *model)
        {
          if (mesh.opaque() && mesh.castShadow()) {
            RenderPlanarShadow(transform, mesh, _lights);
          }
        }
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
    mirror.first.writeToEffect();
    mirror.second->render();

    // b. Reverse all lights. (TODO)
    const auto plane = Math::Plane(mirror.first.position(), mirror.first.upVec());
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
      Transform::writeToEffect(object.transform().matrix() * reflect);
      for (const auto& mesh : *model)
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
      transparent.first.writeToEffect();
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

    mirror.first.writeToEffect();
    mirror.second->render();
  }

  // 4. Sort transparent.

  auto _transparents = transparents();
  const auto camera_pos = _camera->transform().position();
  std::sort(_transparents.begin(), _transparents.end(),
    std::bind(SortBasedOnDistanceToCamera, camera_pos, _1, _2));

  // 5. Render transparent with blending.

  for (const auto& transparent : _transparents) {
    transparent.first.writeToEffect();
    transparent.second->render();
  }

  throwIfFailed(s_graphicsDevice->swapChain()->Present(0, 0));
}

const GameObject * Scene::camera() const
{
  return camera_;
}

std::vector<const GameObject*> Scene::lights()
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

void Scene::retrieveCamera()
{
  camera_ = nullptr;

  for (const auto& object : RecursiveSceneIterator(*this)) {
    auto camera = object.component<Graphics::Camera>();
    if (camera)
      camera_ = &object;
  }
}

void Scene::calculatePicking()
{
  picking_ = nullptr;

  if (camera() != nullptr) {
    if (Input::Mouse::isHeldingDown(Input::Mouse::kLeft)) {

      // Change mouse position from screen space to view space.
      const auto posSS = Input::Mouse::position();
      const auto clientSize = window_->clientRect().GetSize();
      const auto& proj = camera()->component<Graphics::Camera>()->projMaxtrix();
      float xVS = (+2.f * posSS.x / clientSize.w - 1.f) / proj(0, 0);
      float yVS = (-2.f * posSS.y / clientSize.h + 1.f) / proj(1, 1);

      // Change ray from view space to world space.
      const auto rayVS = Math::Ray(Math::Vector3::Zero, Math::Vector3(xVS, yVS, 1.f));
      auto rayWS = rayVS;
      const auto& view = camera()->component<Graphics::Camera>()->viewMatrix();
      rayWS.position = Math::Vector3::Transform(rayWS.position, view.Invert());
      rayWS.direction = Math::Vector3::TransformNormal(rayWS.direction, view.Invert());

      // Walks every object.
      float dist;
      for (const auto& object : RecursiveSceneIterator(*this)) {
        const auto* model = object.component<Graphics::ModelRender>();
        if (model) {

          // Change ray from world space to local space.
          auto rayLS = rayWS;
          const auto world = object.transform().matrix();
          rayLS.position = Math::Vector3::Transform(rayLS.position, world.Invert());
          rayLS.direction = Math::Vector3::TransformNormal(rayLS.direction, world.Invert());
          // Make the ray direction unit length for the intersection tests.
          rayLS.direction.Normalize();

          // Check if ray intersects with object.
          float tmp;
          if (rayLS.Intersects(model->localBoundingBox(), tmp)) {
            for (const auto& meshRender : *model) {
              const auto& mesh = meshRender.mesh();
              for (int i = 2; i < mesh.indices.size(); i += 3) {
                if (rayLS.Intersects(
                  mesh.vertices[mesh.indices[i - 2]].position,
                  mesh.vertices[mesh.indices[i - 1]].position,
                  mesh.vertices[mesh.indices[i]].position,
                  tmp)) {
                  if (!picking_ || tmp < dist) {
                    picking_ = &object;
                    dist = tmp;
                  }
                }
              }
            }
          }
        }
      }
    }
  }
}

std::vector<RenderableMesh> Scene::mirrors()
{
  auto result = std::vector<RenderableMesh>();

  for (const auto& object : RecursiveSceneIterator(*this)) {
    auto model = object.component<Graphics::ModelRender>();
    if (!model)
      continue;
    const auto& transform = object.transform();
    for (const auto& mesh : *model)
    {
      if (mesh.mirror())
        result.push_back(RenderableMesh(transform, &mesh));
    }
  }

  return result;
}

std::vector<RenderableMesh> Scene::transparents()
{
  auto result = std::vector<RenderableMesh>();

  for (const auto& object : RecursiveSceneIterator(*this)) {
    const auto* model = object.component<Graphics::ModelRender>();
    if (!model)
      continue;
    const auto& transform = object.transform();
    for (const auto& mesh : *model)
    {
      if (mesh.transparent())
        result.push_back(RenderableMesh(transform, &mesh));
    }
  }

  return result;
}

}  // namespace LL3D
