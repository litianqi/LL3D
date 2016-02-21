#pragma once

#include <vector>
#include <chrono>
#include <D3D11.h>
#include <d3dx11effect.h>
#include "Camera.h"
#include "Model.h"
#include "Type.h"
#include "Light.h"

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
  Engine(Window* window, const Camera* camera);
  ~Engine();

  void Update(std::chrono::milliseconds dt) {}
  void Draw();

  void SetCamera(const Camera* camera);
  void SetModels(const std::vector<Model>& models);
  void SetLights(const Lights& lights);

  void OnResize();

private:
  void InitDX();
  void LoadEffectFile();
  void CreateInputLayout();
  
  ID3D11Device* dx_device_;
  ID3D11DeviceContext* dx_context_;
  IDXGISwapChain* dx_swap_chain_;
  ID3D11Texture2D* dx_depth_stencil_buffer_;
  ID3D11RenderTargetView* dx_render_target_view_;
  ID3D11DepthStencilView* dx_depth_stencil_view_;
  D3D11_VIEWPORT dx_viewport_;
  ID3D11InputLayout* dx_input_layout_;
  ID3D11Buffer* dx_vertex_buffer_;
  ID3D11Buffer* dx_index_buffer_;
  Model::Mesh dx_mesh_;
  
  ID3DX11EffectTechnique* fx_tech_;
  ID3DX11Effect* fx_effect_;
  
  // Per Frame:
  ID3DX11EffectVariable* fx_ambient_light_;
  ID3DX11EffectVariable* fx_directional_light_;
  ID3DX11EffectVariable* fx_point_light_;
  ID3DX11EffectVariable* fx_spot_light_;
  ID3DX11EffectVectorVariable* fx_eye_pos_w_;
  
  // Per Object:
  ID3DX11EffectMatrixVariable* fx_wvp_;
  ID3DX11EffectMatrixVariable* fx_world_;
  ID3DX11EffectVariable* fx_material_;

  Window* window_;
  const Camera* camera_;
  std::vector<Model> models_;
  Lights lights_;
  
  bool enable_4x_msaa_ = true;
  UINT msaa_quality_;  
};


}  // namespace LL3D