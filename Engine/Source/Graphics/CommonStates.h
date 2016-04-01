#pragma once

struct ID3D11Device;
struct ID3D11BlendState;
struct ID3D11DepthStencilState;
struct ID3D11RasterizerState;
struct ID3D11SamplerState;

namespace LL3D {
namespace Graphics {
namespace CommonStates {

//>
// Initialize CommonStates, called by Base::Init
//
void Initialize(ID3D11Device* device);

//--------------------------------------
// Blend states.
ID3D11BlendState* Opaque();  // Makes no sense, disable blending == Opaque, and 
                             // has better performance.
ID3D11BlendState* AlphaBlend();
ID3D11BlendState* Additive();
ID3D11BlendState* NonPremultiplied();

//--------------------------------------
// Depth stencil states.
ID3D11DepthStencilState* DepthNone();
ID3D11DepthStencilState* DepthDefault();
ID3D11DepthStencilState* DepthRead();
//>
// Set MakMirror before render mirror, so visible part of mirror in stencil 
// buffer will be replaced to StencilRef, others will be 0.
//
ID3D11DepthStencilState* MarkMirror();
//>
// Set RenderReflection before render reflection (in mirror), and StencilRef
// to the same StencilRef used in MarkMirror. So only objects inside mirror
// scope allowed to render.
//
ID3D11DepthStencilState* RenderReflection();

//--------------------------------------
// Rasterizer states.
ID3D11RasterizerState* CullNone();
ID3D11RasterizerState* CullClockwise();
ID3D11RasterizerState* CullCounterClockwise();
ID3D11RasterizerState* Wireframe();

//--------------------------------------
// Sampler states.
ID3D11SamplerState* PointWrap();
ID3D11SamplerState* PointClamp();
ID3D11SamplerState* LinearWrap();
ID3D11SamplerState* LinearClamp();
ID3D11SamplerState* AnisotropicWrap();
ID3D11SamplerState* AnisotropicClamp();

}  // namespace CommonStates
}  // namespace Graphics
}  // namespace LL3D