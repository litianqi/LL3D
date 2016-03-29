#include "Scene.h"
#include "Core\Assert.h"
#include "Core\Exceptions.h"
#include "Graphics\Device.h"
#include "Graphics\Effects.h"
#include "Graphics\Model.h"
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

  // Rendering defer-rendering Models

  auto models = GetDeferRenderingModels();
  auto pos_camera = GetCamera()->GetComponent<Transform>()->GetWorldPosition();
  std::sort(models.begin(), models.end(), 
    [pos_camera](const auto lhs, const auto rhs)
  {
    auto pos_lhs = lhs->GetGameObject()->GetComponent<Transform>()->GetWorldPosition();
    auto pos_rhs = rhs->GetGameObject()->GetComponent<Transform>()->GetWorldPosition();
    auto d_lhs = Math::Vector3::Distance(pos_camera, pos_lhs);
    auto d_rhs = Math::Vector3::Distance(pos_camera, pos_rhs);
    if (d_lhs != d_rhs)
      return d_lhs < d_rhs;
    return false;
  });

  for (auto& model : models)
  {
    model->Update();
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

std::vector<Component*> Scene::GetDeferRenderingModels() noexcept
{
  std::vector<Component*> result;

  /*for (auto& object : RecursiveSceneIterator(*this)) {
    auto model = object.GetComponent<Graphics::Model>();
    if (model && model->GetMaterial().diffuse.A() < 1.0f)
      result.push_back(model);
  }*/

  return result;
}

}  // namespace LL3D
