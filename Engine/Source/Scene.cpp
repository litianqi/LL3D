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
  auto lhs_pos = lhs.first.GetPosition();
  auto rhs_pos = rhs.first.GetPosition();
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
  const Graphics::MeshRender& mesh, const std::vector<GameObject*>& lights)
{
  const auto xz = Math::Plane(Math::Vector3(0.f, 0.f, 0.f), Math::Vector3::Up);
  auto mat = Graphics::Material();
  mat.diffuse = Math::Vector3(0.2f);
  mat.emissive = Math::Vector3::One;
  mat.transparent = Math::Vector3::One;
  mat.shininess = 5.f;
  mat.opacity = 0.5f;
  mat.shininess_strength = 1.f;
  mat.is_shadow = true;

  Math::Vector4 vec;

  for (const auto& light : lights) {
    const auto* light_component = light->GetComponent<Graphics::LightComponent>();
    switch (light_component->GetLightType()) {
    case Graphics::LightComponent::Ambient: {
      continue;
    }
    case Graphics::LightComponent::Directional: {
      const auto dir = -light->GetTransform().GetDirection();
      vec = Math::Vector4(dir.x, dir.y, dir.z, 0.f);
      break;
    }
    case Graphics::LightComponent::Point:
    case Graphics::LightComponent::Spot: {
      const auto pos = light->GetTransform().GetPosition();
      vec = Math::Vector4(pos.x, pos.y, pos.z, 1.f);
      continue;
    }
    default: {
      ASSERT(!"Invalid light type, This is not a valid LightComponent");
    }
    }

    const auto shadow = Math::Matrix::CreateShadow(vec, xz);
    const auto& world = transform.GetMatrix();
    Transform::Render(world * shadow * Math::Matrix::CreateTranslation(ShadowOffset));
    const auto old_mat = mesh.GetMaterial();
    // We lied, but we will recover it latter, it's a good lie.
    const_cast<Graphics::MeshRender&>(mesh).SetMaterial(mat);
    mesh.Render();
    // Recover old material like I said.
    const_cast<Graphics::MeshRender&>(mesh).SetMaterial(old_mat);
  }
}

}  // namespace

namespace LL3D {

void Scene::AddGameObject(const GameObject& object) {
  auto tmp = object;
  tmp.SetScene(this);
  objects_.push_back(tmp);
}

void LL3D::Scene::Update() {

  if (first_update_) {
    for (auto& object : objects_) {
      object.Start();
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
    object.Update();
  }
}

//>
// For a detailed description of Render Procedure, reference to online doc:
// https://onedrive.live.com/redir?page=view&resid=CD6518D498235073!2141&authkey=!AMsU_BK42yKOQEU&wd=target%28Design.one%7C026CF69E-4746-4048-AE49-7E0AEFD55322%2FRender%20Procedure%7C7AB7AF3B-7E41-435E-91FB-99E5CEDC60DC%2F%29
//
void Scene::Render() noexcept
{
  s_graphics_device->GetDeviceContex()->ClearRenderTargetView(
    s_graphics_device->GetRenderTargetView(),
    reinterpret_cast<const float*>(&Math::Color::Grey)
    );
  s_graphics_device->GetDeviceContex()->ClearDepthStencilView(
    s_graphics_device->GetDepthStencilView(),
    D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0, 0
    );

  s_graphics_device->GetDeviceContex()->IASetInputLayout(s_input_layout.Get());
  s_graphics_device->GetDeviceContex()->IASetPrimitiveTopology(
    D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
    );

  // 0. Apply camera.

 const auto* camera = GetCamera();
  if (!camera)
    return;
  camera->GetComponent<Transform>()->Render();
  camera->GetComponent<Graphics::Camera>()->Render();

  // 1. Apply lights.
  
  auto lights = GetLights();
  for (const auto* light : lights) {
    light->GetComponent<Transform>()->Render();
    light->GetComponent<Graphics::LightComponent>()->Render();
  }

  // 2. Render all opaque.

  // a. Render model itself.
  s_graphics_device->GetDeviceContex()->OMSetBlendState(
    nullptr, nullptr, 0xffffffff
    );

  for (const auto& object : RecursiveSceneIterator(*this)) {
    const auto* model = object.GetComponent<Graphics::ModelRender>();
    if (!model)
      continue;
    const auto& transform = object.GetTransform();
    transform.Render();
    for (const auto& mesh : model->GetMeshRenders())
    {
      if (mesh.IsOpaque()) {
        mesh.Render();
      }
    }
  }

  // b. Render model's shadow with blending and stenciling.
  s_graphics_device->GetDeviceContex()->OMSetDepthStencilState(
    Graphics::CommonStates::Shadow(), ShadowStencilRef);
  s_graphics_device->GetDeviceContex()->OMSetBlendState(
    Graphics::CommonStates::Multiply(), nullptr, 0xffffffff
    );

  for (const auto& object : RecursiveSceneIterator(*this)) {
    // Prevent earth have shadow (TODO: improve this)
    if (object.GetName() == "Earth")  
      continue;

    const auto* model = object.GetComponent<Graphics::ModelRender>();
    if (!model)
      continue;

    const auto& transform = object.GetTransform();
    for (const auto& mesh : model->GetMeshRenders())
    {
      if (mesh.IsOpaque()) {
        RenderPlanarShadow(transform, mesh, lights);
      }
    }
  }

  // 3. For each mirror:

  for (const auto& mirror : GetMirrors()) {
    // a. Mark mirror.
    s_graphics_device->GetDeviceContex()->OMSetBlendState(
      nullptr, nullptr, 0xffffffff
      );
    s_graphics_device->GetDeviceContex()->OMSetDepthStencilState(
      Graphics::CommonStates::MarkMirror(), MirrorStencilRef
      );
    mirror.first.Render();
    mirror.second->Render();

    // b. Reverse all lights. (TODO)
    s_graphics_device->GetDeviceContex()->RSSetState(  // Reverse changes winding 
      Graphics::CommonStates::CullClockwise());        // order, so cull clockwise.
    s_graphics_device->GetDeviceContex()->OMSetDepthStencilState(
      Graphics::CommonStates::RenderReflection(), MirrorStencilRef);
    const auto plane = Math::Plane(mirror.first.GetPosition(), mirror.first.GetDirection());
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
    for (const auto& object : RecursiveSceneIterator(*this)) {
      const auto* model = object.GetComponent<Graphics::ModelRender>();
      if (!model)
        continue;
      Transform::Render(object.GetComponent<Transform>()->GetMatrix() * reflect);
      for (const auto& mesh : model->GetMeshRenders())
      {
        if (mesh.IsOpaque() || (mesh.IsMirror() && &mesh != mirror.second))
          mesh.Render();
      }
    }

    // e. Reverse all transparents.
    // f. Sort reversed transparent based on distance to active Camera.
    auto transparents = GetTransparents();
    for (auto& transparent : transparents) {
      transparent.first.SetMatrix(transparent.first.GetMatrix() * reflect);
    }
    const auto camera_pos = camera->GetComponent<Transform>()->GetPosition();
    std::sort(transparents.begin(), transparents.end(),
      std::bind(SortBasedOnDistanceToCamera, camera_pos, _1, _2));

    // g. Render sorted transparent with stenciling and blending.
    s_graphics_device->GetDeviceContex()->OMSetBlendState(
      Graphics::CommonStates::AlphaBlend(), nullptr, 0xffffffff
      );
    for (const auto& transparent : transparents) {
      transparent.first.Render();
      transparent.second->Render();
    }

    // h. Restore lights. (TODO)
    /*for (auto light : lights) {
    light->GetComponent<Graphics::LightComponent>()->Update();
    }*/

    // i. Render current mirror with blending.
    s_graphics_device->GetDeviceContex()->RSSetState(nullptr);
    s_graphics_device->GetDeviceContex()->OMSetDepthStencilState(
      nullptr, 1);

    mirror.first.Render();
    mirror.second->Render();
  }

  // 4. Sort transparent.

  auto transparents = GetTransparents();
  const auto camera_pos = camera->GetComponent<Transform>()->GetPosition();
  std::sort(transparents.begin(), transparents.end(),
    std::bind(SortBasedOnDistanceToCamera, camera_pos, _1, _2));

  // 5. Render transparent with blending.

  for (const auto& transparent : transparents) {
    transparent.first.Render();
    transparent.second->Render();
  }

  ThrowIfFailed(s_graphics_device->GetSwapChain()->Present(0, 0));
}

GameObject * Scene::GetCamera() noexcept
{
  for(auto& object : objects_) {
    auto camera = object.GetComponent<Graphics::Camera>();
    if (camera)
      return &object;
  }
  return nullptr;
}

std::vector<GameObject*> Scene::GetLights() noexcept
{
  auto result = std::vector<GameObject*>();

  for (auto& object : RecursiveSceneIterator(*this)) {
    auto light = object.GetComponent<Graphics::LightComponent>();
    if (light) {
      result.push_back(&object);
    }
  }

  return result;
}

std::vector<RenderableMesh> Scene::GetMirrors() noexcept
{
  auto result = std::vector<RenderableMesh>();

  for (const auto& object : RecursiveSceneIterator(*this)) {
    auto model = object.GetComponent<Graphics::ModelRender>();
    if (!model)
      continue;
    auto transform = object.GetComponent<Transform>();
    for (const auto& mesh : model->GetMeshRenders())
    {
      if (mesh.IsMirror())
        result.push_back(RenderableMesh(*transform, &mesh));
    }
  }

  return result;
}

std::vector<RenderableMesh> Scene::GetTransparents() noexcept
{
  auto result = std::vector<RenderableMesh>();

  for (const auto& object : RecursiveSceneIterator(*this)) {
    auto model = object.GetComponent<Graphics::ModelRender>();
    if (!model)
      continue;
    auto transform = object.GetComponent<Transform>();
    for (const auto& mesh : model->GetMeshRenders())
    {
      if (mesh.IsTransparent())
        result.push_back(RenderableMesh(*transform, &mesh));
    }
  }

  return result;
}

}  // namespace LL3D
