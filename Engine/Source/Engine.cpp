#include "Engine.h"
#include <exception>
#include <D3D11.h>
#include <d3dx11effect.h>
#include <plog\Log.h>
#include "Window.h"
#include "Debug.h"
#include "Color.h"
#include "UIEvents.h"
#include "Model.h"

namespace LL3D {

Engine::Engine(Window* window, const Camera* camera) :
  window_(window),
  camera_(camera)
{
  InitDirectX11();
  // TODO: remove hard code.
  effect_.reset(new BasicEffect(dx_device_, "../x64/Debug/Shaders/Main.fxo"));
  input_layout_.reset(new Vertex::InputLayout(dx_device_, *effect_));
}

Engine::~Engine() {
  dx_vertex_buffer_->Release();
  dx_index_buffer_->Release();
  dx_render_target_view_->Release();
  dx_depth_stencil_view_->Release();
  dx_swap_chain_->Release();
  dx_depth_stencil_buffer_->Release();

  // Restore all default settings.
  if (dx_context_)
    dx_context_->ClearState();
  dx_context_->Release();

  dx_device_->Release();
}

void Engine::Render() {
  dx_context_->ClearRenderTargetView(dx_render_target_view_, reinterpret_cast<const float*>(&Colors::Grey));
  dx_context_->ClearDepthStencilView(dx_depth_stencil_view_, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0, 0); 

  dx_context_->IASetInputLayout(*input_layout_);
  dx_context_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

  UINT stride = sizeof(Vertex);
  UINT offset = 0;
  dx_context_->IASetVertexBuffers(0, 1, &dx_vertex_buffer_, &stride, &offset);
  dx_context_->IASetIndexBuffer(dx_index_buffer_, DXGI_FORMAT_R32_UINT, 0);

  // Set per frame constant buffer.
  effect_->SetLights(lights_);
  effect_->SetEyePosW(camera_->GetPosition());

  for (UINT pass = 0; pass < effect_->GetPassNum(); ++pass) {
    UINT index_count = 0;
    UINT vertex_count = 0;
    UINT index_begin = 0;
    UINT vertex_begin = 0;
    
    for (const auto& model : models_) {
      // Set per object constant buffer.
      effect_->SetWorldMatrix(model.world);
      effect_->SetWVPMatrix(model.world * camera_->GetViewProjectionMatrix());
      effect_->SetMaterial(model.material);

      // Draw object.
      index_begin += index_count;
      vertex_begin += vertex_count;
      index_count = static_cast<UINT>(model.mesh.indices.size());
      vertex_count = static_cast<UINT>(model.mesh.vertices.size());
     
      effect_->GetPass(pass)->Apply(0, dx_context_);
      dx_context_->DrawIndexed(index_count, index_begin, vertex_begin);
    }
  }

  dx_context_->Draw(1, 0);
  HR(dx_swap_chain_->Present(0, 0));
}

void Engine::SetCamera(const Camera * camera) {
  camera_ = camera;
}

void Engine::SetModels(const std::vector<Model>& models) {
  // Cached for Draw;
  models_ = models;
  
  dx_mesh_ = CombineMeshes(models);

  D3D11_BUFFER_DESC vbd;
  vbd.Usage = D3D11_USAGE_IMMUTABLE;
  vbd.ByteWidth = static_cast<UINT>(sizeof(Vertex) * dx_mesh_.vertices.size());
  vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
  vbd.CPUAccessFlags = 0;
  vbd.MiscFlags = 0;
  D3D11_SUBRESOURCE_DATA vinitData;
  vinitData.pSysMem = dx_mesh_.vertices.data();
  HR(dx_device_->CreateBuffer(&vbd, &vinitData, &dx_vertex_buffer_));

  D3D11_BUFFER_DESC ibd;
  ibd.Usage = D3D11_USAGE_IMMUTABLE;
  ibd.ByteWidth = static_cast<UINT>(sizeof(unsigned int) * dx_mesh_.indices.size());
  ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
  ibd.CPUAccessFlags = 0;
  ibd.MiscFlags = 0;
  D3D11_SUBRESOURCE_DATA iinitData;
  iinitData.pSysMem = dx_mesh_.indices.data();
  HR(dx_device_->CreateBuffer(&ibd, &iinitData, &dx_index_buffer_));
}

void Engine::SetLights(const Lights& lights) {
  lights_ = lights;
}

void Engine::OnResize() {
  ASSERT(dx_context_);
  ASSERT(dx_device_);
  ASSERT(dx_swap_chain_);

  // Release the old views, as they hold references to the buffers we
  // will be destroying.  Also release the old depth/stencil buffer.
  if (dx_render_target_view_)
    dx_render_target_view_->Release();
  if (dx_depth_stencil_view_)
    dx_depth_stencil_view_->Release();
  if (dx_depth_stencil_buffer_)
    dx_depth_stencil_buffer_->Release();


  // Resize the swap chain and recreate the render target view.

  const int w_window = window_->GetClientRect().GetSize().w;
  const int h_window = window_->GetClientRect().GetSize().h;
  HR(dx_swap_chain_->ResizeBuffers(1, w_window, h_window,
    DXGI_FORMAT_R8G8B8A8_UNORM, 0));
  ID3D11Texture2D* back_buffer;
  HR(dx_swap_chain_->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&back_buffer)));
  HR(dx_device_->CreateRenderTargetView(back_buffer, 0, &dx_render_target_view_));
  back_buffer->Release();

  // Create the depth/stencil buffer and view.

  D3D11_TEXTURE2D_DESC depth_stencil_desc;

  depth_stencil_desc.Width = w_window;
  depth_stencil_desc.Height = h_window;
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

  HR(dx_device_->CreateTexture2D(&depth_stencil_desc, 0, &dx_depth_stencil_buffer_));
  HR(dx_device_->CreateDepthStencilView(dx_depth_stencil_buffer_, 0, &dx_depth_stencil_view_));


  // Bind the render target view and depth/stencil view to the pipeline.

  dx_context_->OMSetRenderTargets(1, &dx_render_target_view_, dx_depth_stencil_view_);


  // Set the viewport transform.

  dx_viewport_.TopLeftX = 0;
  dx_viewport_.TopLeftY = 0;
  dx_viewport_.Width = static_cast<float>(w_window);
  dx_viewport_.Height = static_cast<float>(h_window);
  dx_viewport_.MinDepth = 0.0f;
  dx_viewport_.MaxDepth = 1.0f;

  dx_context_->RSSetViewports(1, &dx_viewport_);
}

void Engine::InitDirectX11() {
  // Create the device and device context.

  UINT createDeviceFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)  
  createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

  D3D_FEATURE_LEVEL featureLevel;
  HR(D3D11CreateDevice(
    0,                 // default adapter
    D3D_DRIVER_TYPE_HARDWARE,
    0,                 // no software device
    createDeviceFlags,
    0, 0,              // default feature level array
    D3D11_SDK_VERSION,
    &dx_device_,
    &featureLevel,
    &dx_context_));

  ASSERT(featureLevel == D3D_FEATURE_LEVEL_11_0);

  // Check 4X MSAA quality support for our back buffer format.
  // All Direct3D 11 capable devices support 4X MSAA for all render 
  // target formats, so we only need to check quality support.

  dx_device_->CheckMultisampleQualityLevels(
    DXGI_FORMAT_R8G8B8A8_UNORM, 4, &msaa_quality_);
  ASSERT(msaa_quality_ > 0);

  // Fill out a DXGI_SWAP_CHAIN_DESC to describe our swap chain.

  DXGI_SWAP_CHAIN_DESC sd;
  sd.BufferDesc.Width = window_->GetClientRect().GetSize().w;
  sd.BufferDesc.Height = window_->GetClientRect().GetSize().h;
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
  sd.OutputWindow = window_->GetHandle();
  sd.Windowed = true;
  sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
  sd.Flags = 0;

  // To correctly create the swap chain, we must use the IDXGIFactory that was
  // used to create the device.  If we tried to use a different IDXGIFactory instance
  // (by calling CreateDXGIFactory), we get an error: "IDXGIFactory::CreateSwapChain: 
  // This function is being called with a device from a different IDXGIFactory."

  IDXGIDevice* dxgiDevice = 0;
  dx_device_->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice);

  IDXGIAdapter* dxgiAdapter = 0;
  dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter);

  IDXGIFactory* dxgiFactory = 0;
  dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory);

  dxgiFactory->CreateSwapChain(dx_device_, &sd, &dx_swap_chain_);

  dxgiDevice->Release();
  dxgiAdapter->Release();
  dxgiFactory->Release();

  // The remaining steps that need to be carried out for d3d creation
  // also need to be executed every time the window is resized.  So
  // just call the OnResize method here to avoid code duplication.

  OnResize();
}

}  // namespace LL3D