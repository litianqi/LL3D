#include "Scene.h"
#include "Core\Assert.h"
#include "Core\Exceptions.h"
#include "Graphics\Device.h"
#include "Graphics\Effects.h"
#include "Graphics\Model.h"
#include "Graphics\Color.h"
#include "Graphics\Camera.h"
#include "Graphics\Model.h"
#include "Graphics\Light.h"

namespace LL3D {

void LL3D::Scene::Update() {
  s_graphics_device->GetDeviceContex()->ClearRenderTargetView(
    s_graphics_device->GetRenderTargetView(), 
    reinterpret_cast<const float*>(&Graphics::Colors::Grey)
    );
  s_graphics_device->GetDeviceContex()->ClearDepthStencilView(
    s_graphics_device->GetDepthStencilView(), 
    D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0, 0
    );

  s_graphics_device->GetDeviceContex()->IASetInputLayout(*s_input_layout);
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

  ThrowIfFailed(s_graphics_device->GetSwapChain()->Present(0, 0));
}

void Scene::AddGameObject(const GameObject& object) {
  auto tmp = object;
  tmp.SetScene(this);
  objects_.push_back(tmp);
}

}  // namespace LL3D
