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
class Window;

class Engine : private Uncopyable {
public:
  Engine(Window* window, const Camera* camera);

  void Update(std::chrono::milliseconds dt) {}
  void Render();

  void SetCamera(const Camera* camera);
  void SetModels(const std::vector<Model>& models);
  void SetLights(const Lights& lights);

  void OnResize();

  ID3D11Device* GetDevice();
private:
  // TODO
 /* class Impl;
  std::unique_ptr<Impl> impl_;*/

  void InitDirectX11();
  
  Microsoft::WRL::ComPtr<ID3D11Device>            dx_device_;
  Microsoft::WRL::ComPtr<ID3D11DeviceContext>     dx_context_;
  Microsoft::WRL::ComPtr<IDXGISwapChain>          dx_swap_chain_;
  Microsoft::WRL::ComPtr<ID3D11Texture2D>         dx_depth_stencil_buffer_;
  Microsoft::WRL::ComPtr<ID3D11RenderTargetView>  dx_render_target_view_;
  Microsoft::WRL::ComPtr<ID3D11DepthStencilView>  dx_depth_stencil_view_;
  D3D11_VIEWPORT                                  dx_viewport_;

  Window*                               window_;
  const Camera*                         camera_;
  std::unique_ptr<BasicEffect>          effect_;
  std::unique_ptr<Vertex::InputLayout>  input_layout_;
  std::vector<Model>                    models_;
  Lights                                lights_;
  
  bool enable_4x_msaa_ = true;
  UINT msaa_quality_;  
};

}  // namespace LL3D