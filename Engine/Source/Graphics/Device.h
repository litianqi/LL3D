#pragma once

#include <D3D11.h>
#include <wrl.h>
#include <json11.hpp>
#include "Core/Types.h"

namespace LL3D {
namespace Graphics {

class Device
{
public:
  Device(json11::Json config, IntSize2 window_size, HWND window_handle);

  void onResize(IntSize2 window_size);

  ID3D11Device* device() const;
  ID3D11DeviceContext* deviceContex() const;
  IDXGISwapChain* swapChain() const;
  ID3D11RenderTargetView* renderTargetView() const;
  ID3D11DepthStencilView* depthStencilView() const;

private:
  Microsoft::WRL::ComPtr<ID3D11Device> device_;
  Microsoft::WRL::ComPtr<ID3D11DeviceContext> context_;
  Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain_;
  Microsoft::WRL::ComPtr<ID3D11Texture2D> depthStencilBuffer_;
  Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView_;
  Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthStencilView_;
  D3D11_VIEWPORT viewport_;

  bool enable4xMsaa_;
  unsigned msaaQuality_;
};

} // namespace Graphics
} // namespace LL3D
