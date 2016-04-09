#include "Math/Viewport.h"
#include <algorithm>
#include "Math/Matrix.h"

using namespace std;

namespace LL3D {
namespace Math {

/****************************************************************************
*
* Viewport
*
****************************************************************************/
//------------------------------------------------------------------------------
// Comparision operators
//------------------------------------------------------------------------------

bool Viewport::operator == (const Viewport& vp) const
{
  return (x == vp.x && y == vp.y
    && width == vp.width && height == vp.height
    && minDepth == vp.minDepth && maxDepth == vp.maxDepth);
}

bool Viewport::operator != (const Viewport& vp) const
{
  return (x != vp.x || y != vp.y
    || width != vp.width || height != vp.height
    || minDepth != vp.minDepth || maxDepth != vp.maxDepth);
}

//------------------------------------------------------------------------------
// Assignment operators
//------------------------------------------------------------------------------

Viewport& Viewport::operator= (const Viewport& vp)
{
  x = vp.x; y = vp.y;
  width = vp.width; height = vp.height;
  minDepth = vp.minDepth; maxDepth = vp.maxDepth;
  return *this;
}

Viewport& Viewport::operator= (const RECT& rct)
{
  x = float(rct.left); y = float(rct.top);
  width = float(rct.right - rct.left);
  height = float(rct.bottom - rct.top);
  minDepth = 0.f; maxDepth = 1.f;
  return *this;
}

Viewport& Viewport::operator= (const D3D11_VIEWPORT& vp)
{
  x = vp.TopLeftX; y = vp.TopLeftY;
  width = vp.Width; height = vp.Height;
  minDepth = vp.MinDepth; maxDepth = vp.MaxDepth;
  return *this;
}

//------------------------------------------------------------------------------
// Viewport operations
//------------------------------------------------------------------------------

float Viewport::AspectRatio() const
{
  if (width == 0.f || height == 0.f)
    return 0.f;

  return (width / height);
}

Vector3 Viewport::Project(const Vector3& p, const Matrix& proj, const Matrix& view, const Matrix& world) const
{
  using namespace DirectX;
  XMVECTOR v = XMLoadFloat3(&p);
  XMMATRIX projection = XMLoadFloat4x4(&proj);
  v = XMVector3Project(v, x, y, width, height, minDepth, maxDepth, projection, view, world);
  Vector3 result;
  XMStoreFloat3(&result, v);
  return result;
}

void Viewport::Project(const Vector3& p, const Matrix& proj, const Matrix& view, const Matrix& world, Vector3& result) const
{
  using namespace DirectX;
  XMVECTOR v = XMLoadFloat3(&p);
  XMMATRIX projection = XMLoadFloat4x4(&proj);
  v = XMVector3Project(v, x, y, width, height, minDepth, maxDepth, projection, view, world);
  XMStoreFloat3(&result, v);
}

Vector3 Viewport::Unproject(const Vector3& p, const Matrix& proj, const Matrix& view, const Matrix& world) const
{
  using namespace DirectX;
  XMVECTOR v = XMLoadFloat3(&p);
  XMMATRIX projection = XMLoadFloat4x4(&proj);
  v = XMVector3Unproject(v, x, y, width, height, minDepth, maxDepth, projection, view, world);
  Vector3 result;
  XMStoreFloat3(&result, v);
  return result;
}

void Viewport::Unproject(const Vector3& p, const Matrix& proj, const Matrix& view, const Matrix& world, Vector3& result) const
{
  using namespace DirectX;
  XMVECTOR v = XMLoadFloat3(&p);
  XMMATRIX projection = XMLoadFloat4x4(&proj);
  v = XMVector3Unproject(v, x, y, width, height, minDepth, maxDepth, projection, view, world);
  XMStoreFloat3(&result, v);
}

static_assert(sizeof(Viewport) == sizeof(D3D11_VIEWPORT), "Size mismatch");
static_assert(FIELD_OFFSET(Viewport, x) == FIELD_OFFSET(D3D11_VIEWPORT, TopLeftX), "Layout mismatch");
static_assert(FIELD_OFFSET(Viewport, y) == FIELD_OFFSET(D3D11_VIEWPORT, TopLeftY), "Layout mismatch");
static_assert(FIELD_OFFSET(Viewport, width) == FIELD_OFFSET(D3D11_VIEWPORT, Width), "Layout mismatch");
static_assert(FIELD_OFFSET(Viewport, height) == FIELD_OFFSET(D3D11_VIEWPORT, Height), "Layout mismatch");
static_assert(FIELD_OFFSET(Viewport, minDepth) == FIELD_OFFSET(D3D11_VIEWPORT, MinDepth), "Layout mismatch");
static_assert(FIELD_OFFSET(Viewport, maxDepth) == FIELD_OFFSET(D3D11_VIEWPORT, MaxDepth), "Layout mismatch");

RECT Viewport::ComputeDisplayArea(DXGI_SCALING scaling, UINT backBufferWidth, UINT backBufferHeight, int outputWidth, int outputHeight)
{
  RECT rct;

  switch (int(scaling))
  {
  case DXGI_SCALING_STRETCH:
    // Output fills the entire window area
    rct.top = 0;
    rct.left = 0;
    rct.right = outputWidth;
    rct.bottom = outputHeight;
    break;

  case 2 /*DXGI_SCALING_ASPECT_RATIO_STRETCH*/:
    // Output fills the window area but respects the original aspect ratio, using pillar boxing or letter boxing as required
    // Note: This scaling option is not supported for legacy Win32 windows swap chains
  {
    assert(backBufferHeight > 0);
    float aspectRatio = float(backBufferWidth) / float(backBufferHeight);

    // Horizontal fill
    float scaledWidth = float(outputWidth);
    float scaledHeight = float(outputWidth) / aspectRatio;
    if (scaledHeight >= outputHeight)
    {
      // Do vertical fill
      scaledWidth = float(outputHeight) * aspectRatio;
      scaledHeight = float(outputHeight);
    }

    float offsetX = (float(outputWidth) - scaledWidth) * 0.5f;
    float offsetY = (float(outputHeight) - scaledHeight) * 0.5f;

    rct.left = static_cast<LONG>(offsetX);
    rct.top = static_cast<LONG>(offsetY);
    rct.right = static_cast<LONG>(offsetX + scaledWidth);
    rct.bottom = static_cast<LONG>(offsetY + scaledHeight);

    // Clip to display window
    rct.left = max<LONG>(0, rct.left);
    rct.top = max<LONG>(0, rct.top);
    rct.right = min<LONG>(outputWidth, rct.right);
    rct.bottom = min<LONG>(outputHeight, rct.bottom);
  }
  break;

  case DXGI_SCALING_NONE:
  default:
    // Output is displayed in the upper left corner of the window area
    rct.top = 0;
    rct.left = 0;
    rct.right = min<UINT>(backBufferWidth, UINT(outputWidth));
    rct.bottom = min<UINT>(backBufferHeight, UINT(outputHeight));
    break;
  }

  return rct;
}

RECT Viewport::ComputeTitleSafeArea(UINT backBufferWidth, UINT backBufferHeight)
{
  float safew = (float(backBufferWidth) + 9.f) / 10.f;
  float safeh = (float(backBufferHeight) + 9.f) / 10.f;

  RECT rct;
  rct.left = static_cast<LONG>(safew);
  rct.top = static_cast<LONG>(safeh);
  rct.right = static_cast<LONG>(float(backBufferWidth) - safew + 0.5f);
  rct.bottom = static_cast<LONG>(float(backBufferHeight) - safeh + 0.5f);

  return rct;
}

}  // namespace LL3D
}  // namespace Math