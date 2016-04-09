#include "Math/Ray.h"
#include "Math/Plane.h"

namespace LL3D {
namespace Math {

/****************************************************************************
*
* Ray
*
****************************************************************************/

//-----------------------------------------------------------------------------
// Comparision operators
//------------------------------------------------------------------------------
bool Ray::operator == (const Ray& r) const
{
  using namespace DirectX;
  XMVECTOR r1p = XMLoadFloat3(&position);
  XMVECTOR r2p = XMLoadFloat3(&r.position);
  XMVECTOR r1d = XMLoadFloat3(&direction);
  XMVECTOR r2d = XMLoadFloat3(&r.direction);
  return XMVector3Equal(r1p, r2p) && XMVector3Equal(r1d, r2d);
}

bool Ray::operator != (const Ray& r) const
{
  using namespace DirectX;
  XMVECTOR r1p = XMLoadFloat3(&position);
  XMVECTOR r2p = XMLoadFloat3(&r.position);
  XMVECTOR r1d = XMLoadFloat3(&direction);
  XMVECTOR r2d = XMLoadFloat3(&r.direction);
  return XMVector3NotEqual(r1p, r2p) && XMVector3NotEqual(r1d, r2d);
}

//-----------------------------------------------------------------------------
// Ray operators
//------------------------------------------------------------------------------

bool Ray::Intersects(const BoundingSphere& sphere, _Out_ float& Dist) const
{
  return sphere.Intersects(position, direction, Dist);
}

bool Ray::Intersects(const BoundingBox& box, _Out_ float& Dist) const
{
  return box.Intersects(position, direction, Dist);
}

bool Ray::Intersects(const Vector3& tri0, const Vector3& tri1, const Vector3& tri2, _Out_ float& Dist) const
{
  return DirectX::TriangleTests::Intersects(position, direction, tri0, tri1, tri2, Dist);
}

bool Ray::Intersects(const Plane& plane, _Out_ float& Dist) const
{
  using namespace DirectX;

  XMVECTOR p = XMLoadFloat4(&plane);
  XMVECTOR dir = XMLoadFloat3(&direction);

  XMVECTOR nd = XMPlaneDotNormal(p, dir);

  if (XMVector3LessOrEqual(XMVectorAbs(nd), g_RayEpsilon))
  {
    Dist = 0.f;
    return false;
  } else
  {
    // t = -(dot(n,origin) + D) / dot(n,dir)
    XMVECTOR pos = XMLoadFloat3(&position);
    XMVECTOR v = XMPlaneDotNormal(p, pos);
    v = XMVectorAdd(v, XMVectorSplatW(p));
    v = XMVectorDivide(v, nd);
    float dist = -XMVectorGetX(v);
    if (dist < 0)
    {
      Dist = 0.f;
      return false;
    } else
    {
      Dist = dist;
      return true;
    }
  }
}

}  // namespace LL3D
}  // namespace Math

