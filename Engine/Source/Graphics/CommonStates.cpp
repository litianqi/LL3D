#include "Graphics/CommonStates.h"
#include <d3d11.h>
#include <wrl.h>
#include "Core/Exceptions.h"

using namespace Microsoft::WRL;
using namespace LL3D;

namespace {

ComPtr<ID3D11BlendState>        s_opaque;
ComPtr<ID3D11BlendState>        s_alphaBlend;
ComPtr<ID3D11BlendState>        s_substract;
ComPtr<ID3D11BlendState>        s_multiply;
ComPtr<ID3D11DepthStencilState> s_markMirror;
ComPtr<ID3D11DepthStencilState> s_renderReflection;
ComPtr<ID3D11DepthStencilState> s_shadow;
ComPtr<ID3D11DepthStencilState> s_depthLessEqual;
ComPtr<ID3D11RasterizerState>   s_cullNone;
ComPtr<ID3D11RasterizerState>   s_cullClockwise;
ComPtr<ID3D11RasterizerState>   s_cullCounterClockwise;

void createOpaque(ID3D11Device * device)
{
  // TODO
}

void createAlphaBlend(ID3D11Device * device)
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

  throwIfFailed(
    device->CreateBlendState(&desc, &s_alphaBlend)
    );
}

void createSubstract(ID3D11Device * device)
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

  throwIfFailed(
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

  throwIfFailed(
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
  
  throwIfFailed(
    device->CreateDepthStencilState(&desc, &s_markMirror)
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
  
  throwIfFailed(
    device->CreateDepthStencilState(&desc, &s_renderReflection)
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

  throwIfFailed(
    device->CreateDepthStencilState(&desc, &s_shadow)
    );
}

void createDepthLessEqual(ID3D11Device * device)
{
  auto desc = D3D11_DEPTH_STENCIL_DESC();
  desc.DepthEnable = true;  // default
  desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;  // default
  desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
  desc.StencilEnable = false;
  // We do not use stenciling, so these settings do not matter.
  desc.StencilReadMask = 0xff;
  desc.StencilWriteMask = 0xff;
  desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
  desc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;
  desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_INCR;
  desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
  desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
  desc.BackFace.StencilFunc = D3D11_COMPARISON_EQUAL;
  desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
  desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;

  throwIfFailed(
    device->CreateDepthStencilState(&desc, &s_depthLessEqual)
    );
}

void CreateCullNone(ID3D11Device * device) {
  auto desc = D3D11_RASTERIZER_DESC();
  desc.FillMode = D3D11_FILL_SOLID;
  desc.CullMode = D3D11_CULL_NONE;
  desc.FrontCounterClockwise = false;
  desc.DepthClipEnable = true;

  throwIfFailed(
    device->CreateRasterizerState(&desc, &s_cullNone)
    );
}

void CreateCullClockwise(ID3D11Device * device) {
  auto desc = D3D11_RASTERIZER_DESC();
  desc.FillMode = D3D11_FILL_SOLID;
  desc.CullMode = D3D11_CULL_BACK;
  desc.FrontCounterClockwise = true;
  desc.DepthClipEnable = true;

  throwIfFailed(
    device->CreateRasterizerState(&desc, &s_cullClockwise)
    );
}

void CreateCullCounterClockwise(ID3D11Device * device) {
  auto desc = D3D11_RASTERIZER_DESC();
  desc.FillMode = D3D11_FILL_SOLID;
  desc.CullMode = D3D11_CULL_BACK;
  desc.FrontCounterClockwise = false;
  desc.DepthClipEnable = true;

  throwIfFailed(
    device->CreateRasterizerState(&desc, &s_cullCounterClockwise)
    );
}

}  // namespace


namespace LL3D {
namespace Graphics {
namespace CommonStates {

void initialize(ID3D11Device * device)
{
  createOpaque(device);
  createAlphaBlend(device);
  createSubstract(device);
  CreateMultiply(device);
  CreateMakrMirror(device);
  CreateRenderReflection(device);
  CreateShadow(device);
  createDepthLessEqual(device);
  CreateCullNone(device);
  CreateCullClockwise(device);
  CreateCullCounterClockwise(device);
}

//ID3D11BlendState * Opaque()
//{
//  // TODO
//  //return s_opaque.Get();
//}

ID3D11BlendState * alphaBlend()
{
  return s_alphaBlend.Get();
}

ID3D11BlendState * substract()
{
  return s_substract.Get();
}

ID3D11BlendState * multiply()
{
  return s_multiply.Get();
}

ID3D11DepthStencilState * markMirror()
{
  return s_markMirror.Get();
}

ID3D11DepthStencilState * renderReflection()
{
  return s_renderReflection.Get();
}

ID3D11DepthStencilState * shadow()
{
  return s_shadow.Get();
}

ID3D11DepthStencilState * depthLessEqual()
{
  return s_depthLessEqual.Get();
}

ID3D11RasterizerState * cullNone()
{
  return s_cullNone.Get();
}

ID3D11RasterizerState * cullClockwise()
{
  return s_cullClockwise.Get();
}

ID3D11RasterizerState * cullCounterClockwise()
{
  return s_cullCounterClockwise.Get();
}

}  // namespace CommonStates
}  // namespace Graphics
}  // namespace LL3D