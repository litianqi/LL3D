#include "Graphics/Device.h"
#include "Core/Exceptions.h"
#include "Core/Assert.h"

namespace LL3D {
namespace Graphics {

Device::Device(json11::Json config, IntSize2 window_size, HWND window_handle):
  enable_4x_msaa_(config["enable_4x_msaa"].bool_value()) {
  // Create the device and device context.

  UINT createDeviceFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)  
  createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

  D3D_FEATURE_LEVEL featureLevel;
  ThrowIfFailed(D3D11CreateDevice(
    0,                 // default adapter
    D3D_DRIVER_TYPE_HARDWARE,
    0,                 // no software device
    createDeviceFlags,
    0, 0,              // default feature level array
    D3D11_SDK_VERSION,
    &device_,
    &featureLevel,
    &context_));

  ASSERT(featureLevel == D3D_FEATURE_LEVEL_11_0);

  // Check 4X MSAA quality support for our back buffer format.
  // All Direct3D 11 capable devices support 4X MSAA for all render 
  // target formats, so we only need to check quality support.

  device_->CheckMultisampleQualityLevels(
    DXGI_FORMAT_R8G8B8A8_UNORM, 4, &msaa_quality_);
  ASSERT(msaa_quality_ > 0);

  // Fill out a DXGI_SWAP_CHAIN_DESC to describe our swap chain.

  DXGI_SWAP_CHAIN_DESC sd;
  sd.BufferDesc.Width = window_size.w;
  sd.BufferDesc.Height = window_size.h;
  sd.BufferDesc.RefreshRate.Numerator = 60;
  sd.BufferDesc.RefreshRate.Denominator = 1;
  sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
  sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

  // Use 4X MSAA? 
  if (enable_4x_msaa_) {
    sd.SampleDesc.Count = 4;
    sd.SampleDesc.Quality = msaa_quality_ - 1;
  }
  // No MSAA
  else {
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
  }

  sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
  sd.BufferCount = 1;
  sd.OutputWindow = window_handle;
  sd.Windowed = true;
  sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
  sd.Flags = 0;

  // To correctly create the swap chain, we must use the IDXGIFactory that was
  // used to create the device.  If we tried to use a different IDXGIFactory instance
  // (by calling CreateDXGIFactory), we get an error: "IDXGIFactory::CreateSwapChain: 
  // This function is being called with a device from a different IDXGIFactory."

  IDXGIDevice* dxgiDevice = 0;
  device_.Get()->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice);

  IDXGIAdapter* dxgiAdapter = 0;
  dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter);

  IDXGIFactory* dxgiFactory = 0;
  dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory);

  dxgiFactory->CreateSwapChain(device_.Get(), &sd, &swap_chain_);

  dxgiDevice->Release();
  dxgiAdapter->Release();
  dxgiFactory->Release();

  // The remaining steps that need to be carried out for d3d creation
  // also need to be executed every time the window is resized.  So
  // just call the OnResize method here to avoid code duplication.

  OnResize(window_size);
}

void Device::OnResize(IntSize2 window_size) {
  ASSERT(context_);
  ASSERT(device_);
  ASSERT(swap_chain_);
  // Release the old views, as they hold references to the buffers we
  // will be destroying.  Also release the old depth/stencil buffer.
  render_target_view_.Reset();
  depth_stencil_view_.Reset();
  depth_stencil_buffer_.Reset();

  // Resize the swap chain and recreate the render target view.

  ThrowIfFailed(swap_chain_->ResizeBuffers(1, window_size.w, window_size.h,
    DXGI_FORMAT_R8G8B8A8_UNORM, 0));
  ID3D11Texture2D* back_buffer;
  ThrowIfFailed(swap_chain_->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&back_buffer)));
  ThrowIfFailed(device_->CreateRenderTargetView(back_buffer, 0, &render_target_view_));
  back_buffer->Release();

  // Create the depth/stencil buffer and view.

  D3D11_TEXTURE2D_DESC depth_stencil_desc;

  depth_stencil_desc.Width = window_size.w;
  depth_stencil_desc.Height = window_size.h;
  depth_stencil_desc.MipLevels = 1;
  depth_stencil_desc.ArraySize = 1;
  depth_stencil_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

  // Use 4X MSAA? --must match swap chain MSAA values.
  if (enable_4x_msaa_) {
    depth_stencil_desc.SampleDesc.Count = 4;
    depth_stencil_desc.SampleDesc.Quality = msaa_quality_ - 1;
  }
  // No MSAA
  else {
    depth_stencil_desc.SampleDesc.Count = 1;
    depth_stencil_desc.SampleDesc.Quality = 0;
  }

  depth_stencil_desc.Usage = D3D11_USAGE_DEFAULT;
  depth_stencil_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
  depth_stencil_desc.CPUAccessFlags = 0;
  depth_stencil_desc.MiscFlags = 0;

  ThrowIfFailed(device_->CreateTexture2D(&depth_stencil_desc, 0, &depth_stencil_buffer_));
  ThrowIfFailed(device_->CreateDepthStencilView(depth_stencil_buffer_.Get(), 0, &depth_stencil_view_));


  // Bind the render target view and depth/stencil view to the pipeline.

  context_->OMSetRenderTargets(1, render_target_view_.GetAddressOf(), depth_stencil_view_.Get());


  // Set the viewport transform.

  viewport_.TopLeftX = 0;
  viewport_.TopLeftY = 0;
  viewport_.Width = static_cast<float>(window_size.w);
  viewport_.Height = static_cast<float>(window_size.h);
  viewport_.MinDepth = 0.0f;
  viewport_.MaxDepth = 1.0f;

  context_->RSSetViewports(1, &viewport_);
}

ID3D11Device * Device::GetDevice() const {
  return device_.Get();
}

ID3D11DeviceContext * Device::GetDeviceContex() const {
  return context_.Get();
}

IDXGISwapChain * Device::GetSwapChain() const {
  return swap_chain_.Get();
}

ID3D11RenderTargetView * Device::GetRenderTargetView() const {
  return render_target_view_.Get();
}

ID3D11DepthStencilView * Device::GetDepthStencilView() const {
  return depth_stencil_view_.Get();
}

}  // namespace Graphics
}  // namespace LL3D
