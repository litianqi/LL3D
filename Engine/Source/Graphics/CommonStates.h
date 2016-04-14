#pragma once

struct ID3D11Device;
struct ID3D11BlendState;
struct ID3D11DepthStencilState;
struct ID3D11RasterizerState;
struct ID3D11SamplerState;

namespace LL3D {
namespace Graphics {
namespace CommonStates {

void initialize(ID3D11Device* device);

// Makes no sense, disabling blending has same effect and better performance.
ID3D11BlendState* opaque();  
ID3D11BlendState* alphaBlend();
ID3D11BlendState* additive();
ID3D11BlendState* substract();
ID3D11BlendState* multiply();
ID3D11BlendState* nonPremultiplied();

ID3D11DepthStencilState* depthNone();
ID3D11DepthStencilState* depthDefault();
ID3D11DepthStencilState* depthRead();
// Set MakMirror before render mirror, so visible part of mirror in stencil 
// buffer will be replaced to StencilRef, others will be 0.
ID3D11DepthStencilState* markMirror();
// Set RenderReflection before render reflection (in mirror), and StencilRef
// to the same StencilRef used in MarkMirror. So only objects inside mirror
// scope allowed to render.
ID3D11DepthStencilState* renderReflection();
ID3D11DepthStencilState* shadow();
ID3D11DepthStencilState* depthLessEqual();

ID3D11RasterizerState* cullNone();
ID3D11RasterizerState* cullClockwise();
ID3D11RasterizerState* cullCounterClockwise();
ID3D11RasterizerState* wireframe();

ID3D11SamplerState* pointWrap();
ID3D11SamplerState* pointClamp();
ID3D11SamplerState* linearWrap();
ID3D11SamplerState* linearClamp();
ID3D11SamplerState* anisotropicWrap();
ID3D11SamplerState* anisotropicClamp();

}  // namespace CommonStates
}  // namespace Graphics
}  // namespace LL3D