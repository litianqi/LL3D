#include "Engine.h"
#include <exception>
#include <D3D11.h>
#include "Window.h"
#include "Core/Assert.h"
#include "Core/Exceptions.h"
#include "Color.h"
#include "Model.h"
#include "Assets.h"
#include "Graphics\GraphicsDevice.h"

namespace LL3D {

Engine::Engine(Graphics::GraphicsDevice* graphics_device, const Camera* camera) :
  graphics_device_(graphics_device),
  camera_(camera)
{
  // TODO: remove hard code.
  effect_.reset(new BasicEffect(graphics_device->GetDevice(), "../Engine/Resource/Shaders/Main.fxo"));
  input_layout_.reset(new Vertex::InputLayout(graphics_device->GetDevice(), effect_.get()));
}

void Engine::Render() {
  graphics_device_->GetDeviceContex()->ClearRenderTargetView(graphics_device_->GetRenderTargetView(), reinterpret_cast<const float*>(&Colors::Grey));
  graphics_device_->GetDeviceContex()->ClearDepthStencilView(graphics_device_->GetDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0, 0);

  graphics_device_->GetDeviceContex()->IASetInputLayout(*input_layout_);
  graphics_device_->GetDeviceContex()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

  // Set per frame constant buffer.
  effect_->SetLights(lights_);
  effect_->SetEyePosW(camera_->GetPosition());
  effect_->SetView(camera_->GetViewMatrix());
  effect_->SetProjection(camera_->GetFrustum().GetProjectionMaxtrix());

  for (auto& model : models_) {
    model.Render(graphics_device_->GetDeviceContex(), effect_.get(), *input_layout_);
  }
  ThrowIfFailed(graphics_device_->GetSwapChain()->Present(0, 0));
}

void Engine::SetCamera(const Camera * camera) {
  camera_ = camera;
}

void Engine::SetModels(const std::vector<Model>& models) {
  models_ = models;
}

void Engine::SetModel(const std::string& id, const Model& model) {
  /*auto it = std::find_if(models_.begin(), models_.end(), [](const auto& m) { return m.id_ == id; });
  if (it != models_.end())
    *it = */
}

void Engine::SetLights(const Lights& lights) {
  lights_ = lights;
}

}  // namespace LL3D