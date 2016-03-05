#pragma once

#include <D3D11.h>
#include <wrl.h>
#include <json11.hpp>
#include "../Core/Types.h"

namespace LL3D {
namespace Graphics {

class GraphicsDevice {
public:
  GraphicsDevice(json11::Json config, IntSize2 window_size, HWND window_handle);
  
  void OnResize(IntSize2 window_size);

  ID3D11Device* GetDevice();
  ID3D11DeviceContext* GetDeviceContex(); 
  IDXGISwapChain* GetSwapChain();
  ID3D11RenderTargetView* GetRenderTargetView();
  ID3D11DepthStencilView* GetDepthStencilView();

private:
  Microsoft::WRL::ComPtr<ID3D11Device>            dx_device_;
  Microsoft::WRL::ComPtr<ID3D11DeviceContext>     dx_context_;
  Microsoft::WRL::ComPtr<IDXGISwapChain>          dx_swap_chain_;
  Microsoft::WRL::ComPtr<ID3D11Texture2D>         dx_depth_stencil_buffer_;
  Microsoft::WRL::ComPtr<ID3D11RenderTargetView>  dx_render_target_view_;
  Microsoft::WRL::ComPtr<ID3D11DepthStencilView>  dx_depth_stencil_view_;
  D3D11_VIEWPORT                                  dx_viewport_;

  bool      enable_4x_msaa_;
  unsigned  msaa_quality_;
};

}  // namespace Graphics
}  // namespace LL3D
