#pragma once

#include <vector>
#include <chrono>
#include <wrl.h>
#include <D3D11.h>
#include "Core\Uncopyable.h"
#include "Camera.h"
#include "Effects.h"
#include "Model.h"
#include "Core/Types.h"
#include "Lights.h"

struct ID3D11Device;
struct ID3D11DeviceContext;
struct IDXGISwapChain;
struct ID3D11Texture2D;
struct ID3D11RenderTargetView;
struct ID3D11DepthStencilView;

namespace LL3D {

using namespace DirectX;

struct MouseButtonEvent;
struct MouseScrollEvent;
namespace Graphics {
class GraphicsDevice;
}

class Engine : private Uncopyable {
public:
  Engine(Graphics::GraphicsDevice* graphics_device, const Camera* camera);

  void Update(std::chrono::milliseconds dt) {}
  void Render();

  void SetCamera(const Camera* camera);
  void SetModels(const std::vector<Model>& models);
  void SetModel(const std::string& id, const Model& model);
  void SetLights(const Lights& lights);

private:
  // TODO
 /* class Impl;
  std::unique_ptr<Impl> impl_;*/

  Graphics::GraphicsDevice*             graphics_device_;
  const Camera*                         camera_;
  std::unique_ptr<BasicEffect>          effect_;
  std::unique_ptr<Vertex::InputLayout>  input_layout_;
  std::vector<Model>                    models_;
  Lights                                lights_;
  
  bool enable_4x_msaa_ = true;
  UINT msaa_quality_;  
};

}  // namespace LL3D