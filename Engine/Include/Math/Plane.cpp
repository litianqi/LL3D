#include "Math/Plane.h"
#include "Math/Matrix.h"
#include "Math/Quaternion.h"

namespace LL3D {
namespace Math {

/****************************************************************************
*
* Plane
*
****************************************************************************/

 Plane::Plane(const Vector3& point1, const Vector3& point2, const Vector3& point3)
{
  using namespace DirectX;
  XMVECTOR P0 = XMLoadFloat3(&point1);
  XMVECTOR P1 = XMLoadFloat3(&point2);
  XMVECTOR P2 = XMLoadFloat3(&point3);
  XMStoreFloat4(this, XMPlaneFromPoints(P0, P1, P2));
}

 Plane::Plane(const Vector3& point, const Vector3& normal)
{
  using namespace DirectX;
  XMVECTOR P = XMLoadFloat3(&point);
  XMVECTOR N = XMLoadFloat3(&normal);
  XMStoreFloat4(this, XMPlaneFromPointNormal(P, N));
}

//------------------------------------------------------------------------------
// Comparision operators
//------------------------------------------------------------------------------

 bool Plane::operator == (const Plane& p) const
{
  using namespace DirectX;
  XMVECTOR p1 = XMLoadFloat4(this);
  XMVECTOR p2 = XMLoadFloat4(&p);
  return XMPlaneEqual(p1, p2);
}

 bool Plane::operator != (const Plane& p) const
{
  using namespace DirectX;
  XMVECTOR p1 = XMLoadFloat4(this);
  XMVECTOR p2 = XMLoadFloat4(&p);
  return XMPlaneNotEqual(p1, p2);
}

//------------------------------------------------------------------------------
// Plane operations
//------------------------------------------------------------------------------

 void Plane::Normalize()
{
  using namespace DirectX;
  XMVECTOR p = XMLoadFloat4(this);
  XMStoreFloat4(this, XMPlaneNormalize(p));
}

 void Plane::Normalize(Plane& result) const
{
  using namespace DirectX;
  XMVECTOR p = XMLoadFloat4(this);
  XMStoreFloat4(&result, XMPlaneNormalize(p));
}

 float Plane::Dot(const Vector4& v) const
{
  using namespace DirectX;
  XMVECTOR p = XMLoadFloat4(this);
  XMVECTOR v0 = XMLoadFloat4(&v);
  return XMVectorGetX(XMPlaneDot(p, v0));
}

 float Plane::DotCoordinate(const Vector3& position) const
{
  using namespace DirectX;
  XMVECTOR p = XMLoadFloat4(this);
  XMVECTOR v0 = XMLoadFloat3(&position);
  return XMVectorGetX(XMPlaneDotCoord(p, v0));
}

 float Plane::DotNormal(const Vector3& normal) const
{
  using namespace DirectX;
  XMVECTOR p = XMLoadFloat4(this);
  XMVECTOR n0 = XMLoadFloat3(&normal);
  return XMVectorGetX(XMPlaneDotNormal(p, n0));
}

//------------------------------------------------------------------------------
// Static functions
//------------------------------------------------------------------------------

 void Plane::Transform(const Plane& plane, const Matrix& M, Plane& result)
{
  using namespace DirectX;
  XMVECTOR p = XMLoadFloat4(&plane);
  XMMATRIX m0 = XMLoadFloat4x4(&M);
  XMStoreFloat4(&result, XMPlaneTransform(p, m0));
}

 Plane Plane::Transform(const Plane& plane, const Matrix& M)
{
  using namespace DirectX;
  XMVECTOR p = XMLoadFloat4(&plane);
  XMMATRIX m0 = XMLoadFloat4x4(&M);

  Plane result;
  XMStoreFloat4(&result, XMPlaneTransform(p, m0));
  return result;
}

 void Plane::Transform(const Plane& plane, const Quaternion& rotation, Plane& result)
{
  using namespace DirectX;
  XMVECTOR p = XMLoadFloat4(&plane);
  XMVECTOR q = XMLoadFloat4(&rotation);
  XMVECTOR X = XMVector3Rotate(p, q);
  X = XMVectorSelect(p, X, g_XMSelect1110); // result.d = plane.d
  XMStoreFloat4(&result, X);
}

 Plane Plane::Transform(const Plane& plane, const Quaternion& rotation)
{
  using namespace DirectX;
  XMVECTOR p = XMLoadFloat4(&plane);
  XMVECTOR q = XMLoadFloat4(&rotation);
  XMVECTOR X = XMVector3Rotate(p, q);
  X = XMVectorSelect(p, X, g_XMSelect1110); // result.d = plane.d

  Plane result;
  XMStoreFloat4(&result, X);
  return result;
}

}  // namespace LL3D
}  // namespace Math
