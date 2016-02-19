#include "Engine.h"
#include <exception>
#include <filesystem>
#include <fstream>
#include <D3D11.h>
#include <plog\Log.h>
#include "Window.h"
#include "Debug.h"
#include "Color.h"
#include "Vertex.h"
#include "Event.h"
#include "Mesh.h"

using namespace DirectX;

namespace LL3D {

Engine::Engine(Window* window) :
  window_(window),
  camera_(
    Camera::Frustum(
      DirectX::XM_PI / 8.0, 
      static_cast<float>(window->GetClientRect().GetSize().w) / window->GetClientRect().GetSize().h, 
      1, 
      1000),
    DirectX::XMVECTOR{ 0.0f, 200.0f, 0, 1.0f }, 
    DirectX::XMVECTOR{ 0.0001f, -1.0f, 0.0001f }),
  editor_camera_(
    Camera::Frustum(
      DirectX::XM_PI / 8.0,
      static_cast<float>(window->GetClientRect().GetSize().w) / window->GetClientRect().GetSize().h,
      1,
      1000),
    DirectX::XMVECTOR{ 0.0f, 100.0f, 100, 1.0f },
    DirectX::XMVECTOR{ 0.0f, -1.0f, -1.0f })
{
  InitDX();
  LoadEffectFile();
  CreateInputLayout();
}

Engine::~Engine() {
  dx_vertex_buffer_->Release();
  dx_index_buffer_->Release();
  dx_effect_->Release();
  dx_input_layout_->Release();
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

void Engine::Draw() {
  dx_context_->ClearRenderTargetView(dx_render_target_view_, reinterpret_cast<const float*>(&Colors::Grey));
  dx_context_->ClearDepthStencilView(
    dx_depth_stencil_view_,
    D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
    1.0,  // depth 
    0);  // stencil

  dx_context_->IASetInputLayout(dx_input_layout_);
  dx_context_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

  UINT stride = sizeof(Vertex);
  UINT offset = 0;
  dx_context_->IASetVertexBuffers(
    0,
    1,
    &dx_vertex_buffer_,
    &stride,
    &offset);

  dx_context_->IASetIndexBuffer(dx_index_buffer_, DXGI_FORMAT_R32_UINT, 0);

  DirectX::XMMATRIX matrix_wvp = editor_camera_.GetViewProjectionMatrix();  // TODO: lack of world matrix.
  dx_matrix_wvp_->SetMatrix(reinterpret_cast<float*>(&matrix_wvp));

  D3DX11_TECHNIQUE_DESC tech_desc;
  dx_tech_->GetDesc(&tech_desc);
  for (UINT i = 0; i < tech_desc.Passes; ++i) {
    dx_tech_->GetPassByIndex(i)->Apply(0, dx_context_);
    dx_context_->DrawIndexed(index_size_, 0, 0);
  }
  dx_context_->Draw(1, 0);
  HR(dx_swap_chain_->Present(0, 0));
}

void Engine::SetMesh(const MeshData& mesh) {
  if (mesh.indices.empty() || mesh.vertices.empty())
    return;
  
  // Cached for Draw;
  index_size_ = static_cast<unsigned int>(mesh.indices.size());

  D3D11_BUFFER_DESC vbd;
  vbd.Usage = D3D11_USAGE_IMMUTABLE;
  vbd.ByteWidth = static_cast<UINT>(sizeof(Vertex) * mesh.vertices.size());
  vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
  vbd.CPUAccessFlags = 0;
  vbd.MiscFlags = 0;
  D3D11_SUBRESOURCE_DATA vinitData;
  vinitData.pSysMem = &mesh.vertices[0];
  HR(dx_device_->CreateBuffer(&vbd, &vinitData, &dx_vertex_buffer_));

  D3D11_BUFFER_DESC ibd;
  ibd.Usage = D3D11_USAGE_IMMUTABLE;
  ibd.ByteWidth = static_cast<UINT>(sizeof(unsigned int) * mesh.indices.size());
  ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
  ibd.CPUAccessFlags = 0;
  ibd.MiscFlags = 0;
  D3D11_SUBRESOURCE_DATA iinitData;
  iinitData.pSysMem = &mesh.indices[0];
  HR(dx_device_->CreateBuffer(&ibd, &iinitData, &dx_index_buffer_));
}

void Engine::OnMouseDown(const MouseButtonEvent & event) {
  pos_mouse_last_ = event.position;
}

void Engine::OnMouseMove(const MouseButtonEvent & event) {
  
  if ((event.button & MouseButton::Left) != 0) {
    
    // Make each pixel correspond to a quarter of a degree.

    float radian_x = DirectX::XMConvertToRadians(0.1f*static_cast<float>(event.position.x - pos_mouse_last_.x));
    float radian_y = DirectX::XMConvertToRadians(0.2f*static_cast<float>(pos_mouse_last_.y - event.position.y));

    // Update camera

    editor_camera_.Yaw(radian_x);
    editor_camera_.Pitch(radian_y);
    /*camera_.LookAround(radian_x);
    camera_.Pitch(radian_y);*/
  }
  else if ((event.button & MouseButton::Right) != 0) {
    // Get diff to last mouse position
    float d_x = 0.05f * (event.position.x - pos_mouse_last_.x);
    float d_y = 0.05f * (pos_mouse_last_.y - event.position.y);

    editor_camera_.MoveLeftRight(d_x);
    editor_camera_.MoveUpDown(d_y);
  }

  pos_mouse_last_ = event.position;
}

void Engine::OnMouseScroll(const MouseScrollEvent & event) {
  editor_camera_.MoveBackForeward(0.05f * event.distance);
}

void Engine::OnResize() {
  Assert(dx_context_);
  Assert(dx_device_);
  Assert(dx_swap_chain_);

  // Change camera aspect ratio.

  const int w_window = window_->GetClientRect().GetSize().w;
  const int h_window = window_->GetClientRect().GetSize().h;

  auto frustum = editor_camera_.GetFrustum();
  frustum.SetAspectRatio(
    static_cast<float>(w_window) / h_window);
  editor_camera_.SetFrustum(frustum);

  // Release the old views, as they hold references to the buffers we
  // will be destroying.  Also release the old depth/stencil buffer.
  if (dx_render_target_view_)
    dx_render_target_view_->Release();
  if (dx_depth_stencil_view_)
    dx_depth_stencil_view_->Release();
  if (dx_depth_stencil_buffer_)
    dx_depth_stencil_buffer_->Release();


  // Resize the swap chain and recreate the render target view.

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

void Engine::InitDX() {
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

  Assert(featureLevel == D3D_FEATURE_LEVEL_11_0);

  // Check 4X MSAA quality support for our back buffer format.
  // All Direct3D 11 capable devices support 4X MSAA for all render 
  // target formats, so we only need to check quality support.

  dx_device_->CheckMultisampleQualityLevels(
    DXGI_FORMAT_R8G8B8A8_UNORM, 4, &msaa_quality_);
  Assert(msaa_quality_ > 0);

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

void Engine::LoadEffectFile() {
  // File -> Memory:
  
  // Open file
  
  std::ifstream file("Effects/color.fxo", std::ios::binary);
  Assert(file.is_open());
  
  // Create buffer equal to file size
  
  auto size = std::experimental::filesystem::file_size("Effects/color.fxo");
  std::vector<char> content(static_cast<unsigned>(size));
  
  // Copy file content to buffer
  
  file.read(&content[0], size);
  
  // Deserialize:
  
  HR(D3DX11CreateEffectFromMemory(&content[0], size,
    0, dx_device_, &dx_effect_));

  dx_matrix_wvp_ = dx_effect_->GetVariableByName("g_matrix_wvp")->AsMatrix();
  dx_tech_ = dx_effect_->GetTechniqueByName("ColorTech");
}

void Engine::CreateInputLayout() {
  // Create the vertex input layout.
  D3D11_INPUT_ELEMENT_DESC vertex_desc[] =
  {
    { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
  };

  // Create the input layout
  D3DX11_PASS_DESC pass_desc;
  dx_tech_->GetPassByIndex(0)->GetDesc(&pass_desc);
  HR(dx_device_->CreateInputLayout(vertex_desc, 2, pass_desc.pIAInputSignature,
    pass_desc.IAInputSignatureSize, &dx_input_layout_));
}

}  // namespace LL3D