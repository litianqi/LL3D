#pragma once

#include <vector>
#include <chrono>
#include <D3D11.h>
#include <d3dx11effect.h>
#include "Camera.h"
#include "EditorCamera.h"
#include "FirstPersonalCamera.h"
#include "Mesh.h"
#include "Type.h"

struct ID3D11Device;
struct ID3D11DeviceContext;
struct IDXGISwapChain;
struct ID3D11Texture2D;
struct ID3D11RenderTargetView;
struct ID3D11DepthStencilView;

namespace LL3D {

struct MouseButtonEvent;
struct MouseScrollEvent;
class Window;

class Engine {
public:
  Engine(Window* window);
  ~Engine();

  void Update(std::chrono::milliseconds dt) {}
 
  void Draw();

  void SetMesh(const std::vector<MeshData>& meshs);

  // Handle mouse inputs:
  void OnMouseDown(const MouseButtonEvent& event);
  void OnMouseUp(const MouseButtonEvent& event) {}
  void OnMouseMove(const MouseButtonEvent& event);
  void OnMouseScroll(const MouseScrollEvent& event);

  void OnResize();

private:
  void InitDX();
  void LoadEffectFile();
  void CreateInputLayout();
  
  Window* window_;
  FirstPersonalCamera camera_;
  EditorCamera editor_camera_;

  ID3D11Device* dx_device_;
  ID3D11DeviceContext* dx_context_;
  IDXGISwapChain* dx_swap_chain_;
  ID3D11Texture2D* dx_depth_stencil_buffer_;
  ID3D11RenderTargetView* dx_render_target_view_;
  ID3D11DepthStencilView* dx_depth_stencil_view_;
  D3D11_VIEWPORT dx_viewport_;
  ID3DX11EffectTechnique* dx_tech_;
  ID3D11InputLayout* dx_input_layout_;
  ID3DX11Effect* dx_effect_;
  ID3DX11EffectMatrixVariable* dx_matrix_wvp_;
  ID3D11Buffer* dx_vertex_buffer_;
  ID3D11Buffer* dx_index_buffer_;

  // Cached for Draw;
  std::vector<MeshData> meshs_;
  bool enable_4x_msaa_ = true;
  UINT msaa_quality_;

  IntPoint2 pos_mouse_last_;
  
};


}  // namespace LL3D