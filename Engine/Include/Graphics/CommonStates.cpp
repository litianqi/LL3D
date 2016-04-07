#include "Graphics/CommonStates.h"
#include <d3d11.h>
#include <wrl.h>
#include "Core/Exceptions.h"

using namespace Microsoft::WRL;
using namespace LL3D;

namespace {

ComPtr<ID3D11BlendState>        s_opaque;
ComPtr<ID3D11BlendState>        s_alpha_blend;
ComPtr<ID3D11BlendState>        s_substract;
ComPtr<ID3D11BlendState>        s_multiply;
ComPtr<ID3D11DepthStencilState> s_mark_mirror;
ComPtr<ID3D11DepthStencilState> s_render_reflection;
ComPtr<ID3D11DepthStencilState> s_shadow;
ComPtr<ID3D11RasterizerState>   s_cull_none;
ComPtr<ID3D11RasterizerState>   s_cull_clockwise;
ComPtr<ID3D11RasterizerState>   s_cull_counter_clockwise;

void CreateOpaque(ID3D11Device * device)
{
  // TODO
}

void CreateAlphaBlend(ID3D11Device * device)
{
  D3D11_BLEND_DESC desc;
  desc.AlphaToCoverageEnable = true;
  desc.IndependentBlendEnable = false;
  
  desc.RenderTarget[0].BlendEnable = true;
  desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
  desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
  desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
  desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_INV_DEST_ALPHA;
  desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
  desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
  desc.RenderTarget[0].RenderTargetWriteMask = 0x0F;

  ThrowIfFailed(
    device->CreateBlendState(&desc, &s_alpha_blend)
    );
}

void CreateSubstract(ID3D11Device * device)
{
  D3D11_BLEND_DESC desc;
  desc.AlphaToCoverageEnable = true;
  desc.IndependentBlendEnable = false;

  desc.RenderTarget[0].BlendEnable = true;
  desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
  desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
  desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_SUBTRACT;
  desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_INV_DEST_ALPHA;
  desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
  desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
  desc.RenderTarget[0].RenderTargetWriteMask = 0x0F;

  ThrowIfFailed(
    device->CreateBlendState(&desc, &s_substract)
    );
}

void CreateMultiply(ID3D11Device * device)
{
  D3D11_BLEND_DESC desc;
  desc.AlphaToCoverageEnable = true;
  desc.IndependentBlendEnable = false;

  desc.RenderTarget[0].BlendEnable = true;
  desc.RenderTarget[0].SrcBlend = D3D11_BLEND_DEST_COLOR;
  desc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
  desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
  desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_DEST_ALPHA;
  desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
  desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
  desc.RenderTarget[0].RenderTargetWriteMask = 0x0F;

  ThrowIfFailed(
    device->CreateBlendState(&desc, &s_multiply)
    );
}

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

void CreateShadow(ID3D11Device * device)
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
  desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_INCR;
  desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
  // We are not rendering backfacing polygons, so these settings do not matter.
  desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
  desc.BackFace.StencilFunc = D3D11_COMPARISON_EQUAL;
  desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
  desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;

  ThrowIfFailed(
    device->CreateDepthStencilState(&desc, &s_shadow)
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
  CreateOpaque(device);
  CreateAlphaBlend(device);
  CreateSubstract(device);
  CreateMultiply(device);
  CreateMakrMirror(device);
  CreateRenderReflection(device);
  CreateShadow(device);
  CreateCullNone(device);
  CreateCullClockwise(device);
  CreateCullCounterClockwise(device);
}

//ID3D11BlendState * Opaque()
//{
//  // TODO
//  //return s_opaque.Get();
//}

ID3D11BlendState * AlphaBlend()
{
  return s_alpha_blend.Get();
}

ID3D11BlendState * Substract()
{
  return s_substract.Get();
}

ID3D11BlendState * Multiply()
{
  return s_multiply.Get();
}

ID3D11DepthStencilState * MarkMirror()
{
  return s_mark_mirror.Get();
}

ID3D11DepthStencilState * RenderReflection()
{
  return s_render_reflection.Get();
}

ID3D11DepthStencilState * Shadow()
{
  return s_shadow.Get();
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