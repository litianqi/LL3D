#pragma once

#include <functional>
#include <d3d11_1.h>
#include <DirectXMath.h>
#include "Math/Vector.h"

namespace LL3D {
namespace Math {

using namespace DirectX;
struct Matrix;

//------------------------------------------------------------------------------
// Viewport
class Viewport
{
public:
  float x;
  float y;
  float width;
  float height;
  float minDepth;
  float maxDepth;

  Viewport() :
    x(0.f), y(0.f), width(0.f), height(0.f), minDepth(0.f), maxDepth(1.f) {}
  Viewport(float ix, float iy, float iw, float ih, float iminz = 0.f, float imaxz = 1.f) :
    x(ix), y(iy), width(iw), height(ih), minDepth(iminz), maxDepth(imaxz) {}
  explicit Viewport(const RECT& rct) :
    x(float(rct.left)), y(float(rct.top)),
    width(float(rct.right - rct.left)),
    height(float(rct.bottom - rct.top)),
    minDepth(0.f), maxDepth(1.f) {}
  explicit Viewport(const D3D11_VIEWPORT& vp) :
    x(vp.TopLeftX), y(vp.TopLeftY),
    width(vp.Width), height(vp.Height),
    minDepth(vp.MinDepth), maxDepth(vp.MaxDepth) {}

  // Direct3D 11 interop
  operator D3D11_VIEWPORT() { return *reinterpret_cast<D3D11_VIEWPORT*>(this); }
  const D3D11_VIEWPORT* Get11() const { return reinterpret_cast<const D3D11_VIEWPORT*>(this); }

  // Comparison operators
  bool operator == (const Viewport& vp) const;
  bool operator != (const Viewport& vp) const;

  // Assignment operators
  Viewport& operator= (const Viewport& vp);
  Viewport& operator= (const RECT& rct);
  Viewport& operator= (const D3D11_VIEWPORT& vp);

  // Viewport operations
  float AspectRatio() const;

  Vector3 Project(const Vector3& p, const Matrix& proj, const Matrix& view, const Matrix& world) const;
  void Project(const Vector3& p, const Matrix& proj, const Matrix& view, const Matrix& world, Vector3& result) const;

  Vector3 Unproject(const Vector3& p, const Matrix& proj, const Matrix& view, const Matrix& world) const;
  void Unproject(const Vector3& p, const Matrix& proj, const Matrix& view, const Matrix& world, Vector3& result) const;

  // Static methods
  static RECT __cdecl ComputeDisplayArea(DXGI_SCALING scaling, UINT backBufferWidth, UINT backBufferHeight, int outputWidth, int outputHeight);
  static RECT __cdecl ComputeTitleSafeArea(UINT backBufferWidth, UINT backBufferHeight);
};

}  // namespace Math
}  // namespace LL3D

//------------------------------------------------------------------------------
// Support for Math and Standard C++ Library containers
namespace std {

template<> struct less<LL3D::Math::Viewport>
{
  bool operator()(const LL3D::Math::Viewport& vp1, const LL3D::Math::Viewport& vp2) const
  {
    if (vp1.x != vp2.x) return (vp1.x < vp2.x);
    if (vp1.y != vp2.y) return (vp1.y < vp2.y);

    if (vp1.width != vp2.width) return (vp1.width < vp2.width);
    if (vp1.height != vp2.height) return (vp1.height < vp2.height);

    if (vp1.minDepth != vp2.minDepth) return (vp1.minDepth < vp2.minDepth);
    if (vp1.maxDepth != vp2.maxDepth) return (vp1.maxDepth < vp2.maxDepth);

    return false;
  }
};

}  // namespace std