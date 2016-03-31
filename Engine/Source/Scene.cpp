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

using namespace std::placeholders;
using namespace LL3D;

namespace {

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

}

namespace LL3D {

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

void Scene::AddGameObject(const GameObject& object) {
  auto tmp = object;
  tmp.SetScene(this);
  objects_.push_back(tmp);
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

//std::vector<GameObject*> Scene::GetLights() noexcept
//{
//  auto result = std::vector<GameObject*>();
//
//  for (auto& object : RecursiveSceneIterator(*this)) {
//    auto light = object.GetComponent<Graphics::LightComponent>();
//    if (light) {
//      result.push_back(&object);
//    }
//  }
//
//  return result;
//}

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

  // 1. Render all opaque.

  for (auto& object : RecursiveSceneIterator(*this)) {
    auto model = object.GetComponent<Graphics::ModelRender>();
    if (!model)
      continue;
    object.GetComponent<Transform>()->Update();
    for (const auto& mesh : model->GetMeshRenders())
    {
      if (mesh.IsOpaque())
        mesh.Render();
    }
  }

  // 2. For each mirror:

  for (auto& mirror : GetMirrors()) {
    // a. Mark mirror.
    s_graphics_device->GetDeviceContex()->OMSetDepthStencilState(
      Graphics::CommonStates::MarkMirror(), 1);
    mirror.first.Update();
    mirror.second->Render();

    // b. Reverse every light and apply. (TODO)
    s_graphics_device->GetDeviceContex()->RSSetState(  // Reverse changes winding 
      Graphics::CommonStates::CullClockwise());        // order, so cull clockwise.
    s_graphics_device->GetDeviceContex()->OMSetDepthStencilState(
      Graphics::CommonStates::RenderReflection(), 1);
    auto plane = Math::Plane(mirror.first.GetPosition(), mirror.first.GetDirection());
    auto reflect = Math::XMMatrixReflect(plane);
    // Raise reversed world a little, otherwise a plane long engough to exist 
    // behind mirror may coincide with plane in mirror. DirectX is random at 
    // which one to use, even you render one of them latter:(
    reflect *= Math::Matrix::CreateTranslation(Math::Vector3(0.f, 0.1f, 0.f));

    /*for (auto light : GetLights()) {
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
      auto model = object.GetComponent<Graphics::ModelRender>();
      if (!model)
        continue;
      auto transform = *object.GetComponent<Transform>();
      transform.SetMatrix(transform.GetMatrix() * reflect);
      transform.Update();
      for (const auto& mesh : model->GetMeshRenders())
      {
        if (mesh.IsOpaque() || (mesh.IsMirror() && &mesh != mirror.second))
          mesh.Render();  // TODO: render mirror without blending
      }
    }

    // e. Reverse all transparents.
    // f. Sort reversed transparent based on distance to active Camera.
    auto transparents = GetTransparents();
    for (auto& transparent : transparents) {
      transparent.first.SetMatrix(transparent.first.GetMatrix() * reflect);
    }
    auto camera_pos = GetCamera()->GetComponent<Transform>()->GetPosition();
    std::sort(transparents.begin(), transparents.end(),
      std::bind(SortBasedOnDistanceToCamera, camera_pos, _1, _2));

    // g. Render sorted transparent with stenciling and blending.
    s_graphics_device->GetDeviceContex()->OMSetBlendState(
      Graphics::CommonStates::AlphaBlend(), nullptr, 0xffffffff
      );
    for (auto& transparent : transparents) {
      transparent.first.Update();
      transparent.second->Render();
    }

    // h. Restore light and apply. (TODO)
    /*for (auto light : GetLights()) {
      light->GetComponent<Graphics::LightComponent>()->Update();
    }*/

    // i. Render current mirror with blending.
    s_graphics_device->GetDeviceContex()->RSSetState(nullptr);
    s_graphics_device->GetDeviceContex()->OMSetDepthStencilState(
      nullptr, 1);
    
    mirror.first.Update();
    mirror.second->Render();
  }

  // 3. Sort transparent.

  auto transparents = GetTransparents();
  auto camera_pos = GetCamera()->GetComponent<Transform>()->GetPosition();
  std::sort(transparents.begin(), transparents.end(),
    std::bind(SortBasedOnDistanceToCamera, camera_pos, _1, _2));

  // 4. Render transparent with blending.

  for (auto& transparent : transparents) {
    transparent.first.Update();
    transparent.second->Render();
  }
  s_graphics_device->GetDeviceContex()->OMSetBlendState(
    nullptr, nullptr, 0xffffffff
    );

  ThrowIfFailed(s_graphics_device->GetSwapChain()->Present(0, 0));
}

}  // namespace LL3D
