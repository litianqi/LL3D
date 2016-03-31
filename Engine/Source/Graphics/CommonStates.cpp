#include "CommonStates.h"
#include <d3d11.h>
#include <wrl.h>
#include "../Core/Exceptions.h"

using namespace Microsoft::WRL;
using namespace LL3D;

namespace {

ComPtr<ID3D11DepthStencilState> s_mark_mirror;
ComPtr<ID3D11DepthStencilState> s_render_reflection;
ComPtr<ID3D11RasterizerState>   s_cull_none;
ComPtr<ID3D11RasterizerState>   s_cull_clockwise;
ComPtr<ID3D11RasterizerState>   s_cull_counter_clockwise;

void CreateMakrMirror(ID3D11Device * device)
{
  auto desc = D3D11_DEPTH_STENCIL_DESC();
  desc.DepthEnable = true;  // default
  desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;  
  desc.DepthFunc = D3D11_COMPARISON_LESS;  // defualt
  desc.StencilEnable = true;
  desc.StencilReadMask = 0xff;
  desc.StencilWriteMask = 0xff;
  desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
  desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
  desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
  // StencilFun always succeed, so this setting doesn't matter.
  desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
  // We are not rendering backfacing polygons, so these settings do not matter.
  desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
  desc.BackFace.StencilFunc = D3D11_COMPARISON_EQUAL;
  desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
  desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
  
  ThrowIfFailed(
    device->CreateDepthStencilState(&desc, &s_mark_mirror)
    );
}

void CreateRenderReflection(ID3D11Device * device)
{
  auto desc = D3D11_DEPTH_STENCIL_DESC();
  desc.DepthEnable = true;  // default
  desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;  // default
  desc.DepthFunc = D3D11_COMPARISON_LESS;  // default
  desc.StencilEnable = true;
  desc.StencilReadMask = 0xff;
  desc.StencilWriteMask = 0xff;
  desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
  desc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;
  desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
  desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
  // We are not rendering backfacing polygons, so these settings do not matter.
  desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
  desc.BackFace.StencilFunc = D3D11_COMPARISON_EQUAL;
  desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
  desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
  
  ThrowIfFailed(
    device->CreateDepthStencilState(&desc, &s_render_reflection)
    );
}

void CreateCullNone(ID3D11Device * device) {
  auto desc = D3D11_RASTERIZER_DESC();
  desc.FillMode = D3D11_FILL_SOLID;
  desc.CullMode = D3D11_CULL_NONE;
  desc.FrontCounterClockwise = false;
  desc.DepthClipEnable = true;

  ThrowIfFailed(
    device->CreateRasterizerState(&desc, &s_cull_none)
    );
}

void CreateCullClockwise(ID3D11Device * device) {
  auto desc = D3D11_RASTERIZER_DESC();
  desc.FillMode = D3D11_FILL_SOLID;
  desc.CullMode = D3D11_CULL_BACK;
  desc.FrontCounterClockwise = true;
  desc.DepthClipEnable = true;

  ThrowIfFailed(
    device->CreateRasterizerState(&desc, &s_cull_clockwise)
    );
}

void CreateCullCounterClockwise(ID3D11Device * device) {
  auto desc = D3D11_RASTERIZER_DESC();
  desc.FillMode = D3D11_FILL_SOLID;
  desc.CullMode = D3D11_CULL_BACK;
  desc.FrontCounterClockwise = false;
  desc.DepthClipEnable = true;

  ThrowIfFailed(
    device->CreateRasterizerState(&desc, &s_cull_counter_clockwise)
    );
}

}  // namespace


namespace LL3D {
namespace Graphics {
namespace CommonStates {

void Initialize(ID3D11Device * device)
{
  CreateMakrMirror(device);
  CreateRenderReflection(device);
  CreateCullNone(device);
  CreateCullClockwise(device);
  CreateCullCounterClockwise(device);
}

ID3D11DepthStencilState * MarkMirror()
{
  return s_mark_mirror.Get();
}

ID3D11DepthStencilState * RenderReflection()
{
  return s_render_reflection.Get();
}

ID3D11RasterizerState * CullNone()
{
  return s_cull_none.Get();
}

ID3D11RasterizerState * CullClockwise()
{
  return s_cull_clockwise.Get();
}

ID3D11RasterizerState * CullCounterClockwise()
{
  return s_cull_counter_clockwise.Get();
}

}  // namespace CommonStates
}  // namespace Graphics
}  // namespace LL3D