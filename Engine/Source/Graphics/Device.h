#pragma once

#include <D3D11.h>
#include <wrl.h>
#include <json11.hpp>
#include "../Core/Types.h"

namespace LL3D {
namespace Graphics {

class Device {
public:
  Device(json11::Json config, IntSize2 window_size, HWND window_handle);
  
  void OnResize(IntSize2 window_size);

  ID3D11Device* GetDevice() const;
  ID3D11DeviceContext* GetDeviceContex() const; 
  IDXGISwapChain* GetSwapChain() const;
  ID3D11RenderTargetView* GetRenderTargetView() const;
  ID3D11DepthStencilView* GetDepthStencilView() const;

private:
  Microsoft::WRL::ComPtr<ID3D11Device>            device_;
  Microsoft::WRL::ComPtr<ID3D11DeviceContext>     context_;
  Microsoft::WRL::ComPtr<IDXGISwapChain>          swap_chain_;
  Microsoft::WRL::ComPtr<ID3D11Texture2D>         depth_stencil_buffer_;
  Microsoft::WRL::ComPtr<ID3D11RenderTargetView>  render_target_view_;
  Microsoft::WRL::ComPtr<ID3D11DepthStencilView>  depth_stencil_view_;
  D3D11_VIEWPORT                                  viewport_;

  bool      enable_4x_msaa_;
  unsigned  msaa_quality_;
};

}  // namespace Graphics
}  // namespace LL3D
