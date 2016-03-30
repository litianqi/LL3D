#include "Scene.h"
#include "Core\Assert.h"
#include "Core\Exceptions.h"
#include "Graphics\Device.h"
#include "Graphics\Effects.h"
#include "Graphics\ModelRender.h"
#include "Graphics\Camera.h"
#include "RecursiveSceneIterator.h"

namespace LL3D {

void LL3D::Scene::Update() {

  if (first_update_) {
    for (auto& object : objects_) {
      object.Start();
    }
    first_update_ = false;
  }

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

  // Renders transparent ModelRenders

  auto transparent_objects = GetTransparentGameObjects();
  auto camera_pos = GetCamera()->GetComponent<Transform>()->GetWorldPosition();
  std::sort(transparent_objects.begin(), transparent_objects.end(), 
    [camera_pos](const auto lhs, const auto rhs)
  {
    auto lhs_pos = lhs->GetComponent<Transform>()->GetWorldPosition();
    auto rhs_pos = rhs->GetComponent<Transform>()->GetWorldPosition();
    auto lhs_d = Math::Vector3::Distance(camera_pos, lhs_pos);
    auto rhs_d = Math::Vector3::Distance(camera_pos, rhs_pos);
    if (lhs_d != rhs_d)
      return lhs_d < rhs_d;
    return false;
  });

  for (auto& object : transparent_objects)
  {
    object->DoUpdate();
  }

  ThrowIfFailed(s_graphics_device->GetSwapChain()->Present(0, 0));
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

std::vector<GameObject*> Scene::GetTransparentGameObjects() noexcept
{
  std::vector<GameObject*> result;

  for (auto& object : RecursiveSceneIterator(*this)) {
    if (object.IsTransparent())
      result.push_back(&object);
  }

  return result;
}

std::vector<Component*> Scene::GetMirrorModelRenders() noexcept
{
  return std::vector<Component*>();
}

}  // namespace LL3D
