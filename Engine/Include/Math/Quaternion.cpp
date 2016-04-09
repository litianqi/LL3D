#include "Math/Quaternion.h"
#include "Math/Matrix.h"

namespace LL3D {
namespace Math {

/****************************************************************************
*
* Quaternion
*
****************************************************************************/

//------------------------------------------------------------------------------
// Comparision operators
//------------------------------------------------------------------------------

bool Quaternion::operator == (const Quaternion& q) const
{
  using namespace DirectX;
  XMVECTOR q1 = XMLoadFloat4(this);
  XMVECTOR q2 = XMLoadFloat4(&q);
  return XMQuaternionEqual(q1, q2);
}

bool Quaternion::operator != (const Quaternion& q) const
{
  using namespace DirectX;
  XMVECTOR q1 = XMLoadFloat4(this);
  XMVECTOR q2 = XMLoadFloat4(&q);
  return XMQuaternionNotEqual(q1, q2);
}

//------------------------------------------------------------------------------
// Assignment operators
//------------------------------------------------------------------------------

Quaternion& Quaternion::operator+= (const Quaternion& q)
{
  using namespace DirectX;
  XMVECTOR q1 = XMLoadFloat4(this);
  XMVECTOR q2 = XMLoadFloat4(&q);
  XMStoreFloat4(this, XMVectorAdd(q1, q2));
  return *this;
}

Quaternion& Quaternion::operator-= (const Quaternion& q)
{
  using namespace DirectX;
  XMVECTOR q1 = XMLoadFloat4(this);
  XMVECTOR q2 = XMLoadFloat4(&q);
  XMStoreFloat4(this, XMVectorSubtract(q1, q2));
  return *this;
}

Quaternion& Quaternion::operator*= (const Quaternion& q)
{
  using namespace DirectX;
  XMVECTOR q1 = XMLoadFloat4(this);
  XMVECTOR q2 = XMLoadFloat4(&q);
  XMStoreFloat4(this, XMQuaternionMultiply(q1, q2));
  return *this;
}

Quaternion& Quaternion::operator*= (float S)
{
  using namespace DirectX;
  XMVECTOR q = XMLoadFloat4(this);
  XMStoreFloat4(this, XMVectorScale(q, S));
  return *this;
}

Quaternion& Quaternion::operator/= (const Quaternion& q)
{
  using namespace DirectX;
  XMVECTOR q1 = XMLoadFloat4(this);
  XMVECTOR q2 = XMLoadFloat4(&q);
  q2 = XMQuaternionInverse(q2);
  XMStoreFloat4(this, XMQuaternionMultiply(q1, q2));
  return *this;
}

//------------------------------------------------------------------------------
// Urnary operators
//------------------------------------------------------------------------------

Quaternion Quaternion::operator- () const
{
  using namespace DirectX;
  XMVECTOR q = XMLoadFloat4(this);

  Quaternion R;
  XMStoreFloat4(&R, XMVectorNegate(q));
  return R;
}

//------------------------------------------------------------------------------
// Binary operators
//------------------------------------------------------------------------------

Quaternion operator+ (const Quaternion& Q1, const Quaternion& Q2)
{
  using namespace DirectX;
  XMVECTOR q1 = XMLoadFloat4(&Q1);
  XMVECTOR q2 = XMLoadFloat4(&Q2);

  Quaternion R;
  XMStoreFloat4(&R, XMVectorAdd(q1, q2));
  return R;
}

Quaternion operator- (const Quaternion& Q1, const Quaternion& Q2)
{
  using namespace DirectX;
  XMVECTOR q1 = XMLoadFloat4(&Q1);
  XMVECTOR q2 = XMLoadFloat4(&Q2);

  Quaternion R;
  XMStoreFloat4(&R, XMVectorSubtract(q1, q2));
  return R;
}

Quaternion operator* (const Quaternion& Q1, const Quaternion& Q2)
{
  using namespace DirectX;
  XMVECTOR q1 = XMLoadFloat4(&Q1);
  XMVECTOR q2 = XMLoadFloat4(&Q2);

  Quaternion R;
  XMStoreFloat4(&R, XMQuaternionMultiply(q1, q2));
  return R;
}

Quaternion operator* (const Quaternion& Q, float S)
{
  using namespace DirectX;
  XMVECTOR q = XMLoadFloat4(&Q);

  Quaternion R;
  XMStoreFloat4(&R, XMVectorScale(q, S));
  return R;
}

Quaternion operator/ (const Quaternion& Q1, const Quaternion& Q2)
{
  using namespace DirectX;
  XMVECTOR q1 = XMLoadFloat4(&Q1);
  XMVECTOR q2 = XMLoadFloat4(&Q2);
  q2 = XMQuaternionInverse(q2);

  Quaternion R;
  XMStoreFloat4(&R, XMQuaternionMultiply(q1, q2));
  return R;
}

Quaternion operator* (float S, const Quaternion& Q)
{
  using namespace DirectX;
  XMVECTOR q1 = XMLoadFloat4(&Q);

  Quaternion R;
  XMStoreFloat4(&R, XMVectorScale(q1, S));
  return R;
}

//------------------------------------------------------------------------------
// Quaternion operations
//------------------------------------------------------------------------------

float Quaternion::Length() const
{
  using namespace DirectX;
  XMVECTOR q = XMLoadFloat4(this);
  return XMVectorGetX(XMQuaternionLength(q));
}

float Quaternion::LengthSquared() const
{
  using namespace DirectX;
  XMVECTOR q = XMLoadFloat4(this);
  return XMVectorGetX(XMQuaternionLengthSq(q));
}

void Quaternion::Normalize()
{
  using namespace DirectX;
  XMVECTOR q = XMLoadFloat4(this);
  XMStoreFloat4(this, XMQuaternionNormalize(q));
}

void Quaternion::Normalize(Quaternion& result) const
{
  using namespace DirectX;
  XMVECTOR q = XMLoadFloat4(this);
  XMStoreFloat4(&result, XMQuaternionNormalize(q));
}

void Quaternion::Conjugate()
{
  using namespace DirectX;
  XMVECTOR q = XMLoadFloat4(this);
  XMStoreFloat4(this, XMQuaternionConjugate(q));
}

void Quaternion::Conjugate(Quaternion& result) const
{
  using namespace DirectX;
  XMVECTOR q = XMLoadFloat4(this);
  XMStoreFloat4(&result, XMQuaternionConjugate(q));
}

void Quaternion::Inverse(Quaternion& result) const
{
  using namespace DirectX;
  XMVECTOR q = XMLoadFloat4(this);
  XMStoreFloat4(&result, XMQuaternionInverse(q));
}

float Quaternion::Dot(const Quaternion& q) const
{
  using namespace DirectX;
  XMVECTOR q1 = XMLoadFloat4(this);
  XMVECTOR q2 = XMLoadFloat4(&q);
  return XMVectorGetX(XMQuaternionDot(q1, q2));
}

//------------------------------------------------------------------------------
// Static functions
//------------------------------------------------------------------------------

Quaternion Quaternion::CreateFromAxisAngle(const Vector3& axis, float angle)
{
  using namespace DirectX;
  XMVECTOR a = XMLoadFloat3(&axis);

  Quaternion R;
  XMStoreFloat4(&R, XMQuaternionRotationAxis(a, angle));
  return R;
}

Quaternion Quaternion::CreateFromYawPitchRoll(float yaw, float pitch, float roll)
{
  using namespace DirectX;
  Quaternion R;
  XMStoreFloat4(&R, XMQuaternionRotationRollPitchYaw(pitch, yaw, roll));
  return R;
}

Quaternion Quaternion::CreateFromRotationMatrix(const Matrix& M)
{
  using namespace DirectX;
  XMMATRIX M0 = XMLoadFloat4x4(&M);

  Quaternion R;
  XMStoreFloat4(&R, XMQuaternionRotationMatrix(M0));
  return R;
}

void Quaternion::Lerp(const Quaternion& q1, const Quaternion& q2, float t, Quaternion& result)
{
  using namespace DirectX;
  XMVECTOR Q0 = XMLoadFloat4(&q1);
  XMVECTOR Q1 = XMLoadFloat4(&q2);

  XMVECTOR dot = XMVector4Dot(Q0, Q1);

  XMVECTOR R;
  if (XMVector4GreaterOrEqual(dot, XMVectorZero()))
  {
    R = XMVectorLerp(Q0, Q1, t);
  } else
  {
    XMVECTOR tv = XMVectorReplicate(t);
    XMVECTOR t1v = XMVectorReplicate(1.f - t);
    XMVECTOR X0 = XMVectorMultiply(Q0, t1v);
    XMVECTOR X1 = XMVectorMultiply(Q1, tv);
    R = XMVectorSubtract(X0, X1);
  }

  XMStoreFloat4(&result, XMQuaternionNormalize(R));
}

Quaternion Quaternion::Lerp(const Quaternion& q1, const Quaternion& q2, float t)
{
  using namespace DirectX;
  XMVECTOR Q0 = XMLoadFloat4(&q1);
  XMVECTOR Q1 = XMLoadFloat4(&q2);

  XMVECTOR dot = XMVector4Dot(Q0, Q1);

  XMVECTOR R;
  if (XMVector4GreaterOrEqual(dot, XMVectorZero()))
  {
    R = XMVectorLerp(Q0, Q1, t);
  } else
  {
    XMVECTOR tv = XMVectorReplicate(t);
    XMVECTOR t1v = XMVectorReplicate(1.f - t);
    XMVECTOR X0 = XMVectorMultiply(Q0, t1v);
    XMVECTOR X1 = XMVectorMultiply(Q1, tv);
    R = XMVectorSubtract(X0, X1);
  }

  Quaternion result;
  XMStoreFloat4(&result, XMQuaternionNormalize(R));
  return result;
}

void Quaternion::Slerp(const Quaternion& q1, const Quaternion& q2, float t, Quaternion& result)
{
  using namespace DirectX;
  XMVECTOR Q0 = XMLoadFloat4(&q1);
  XMVECTOR Q1 = XMLoadFloat4(&q2);
  XMStoreFloat4(&result, XMQuaternionSlerp(Q0, Q1, t));
}

Quaternion Quaternion::Slerp(const Quaternion& q1, const Quaternion& q2, float t)
{
  using namespace DirectX;
  XMVECTOR Q0 = XMLoadFloat4(&q1);
  XMVECTOR Q1 = XMLoadFloat4(&q2);

  Quaternion result;
  XMStoreFloat4(&result, XMQuaternionSlerp(Q0, Q1, t));
  return result;
}

void Quaternion::Concatenate(const Quaternion& q1, const Quaternion& q2, Quaternion& result)
{
  using namespace DirectX;
  XMVECTOR Q0 = XMLoadFloat4(&q1);
  XMVECTOR Q1 = XMLoadFloat4(&q2);
  XMStoreFloat4(&result, XMQuaternionMultiply(Q1, Q0));
}

Quaternion Quaternion::Concatenate(const Quaternion& q1, const Quaternion& q2)
{
  using namespace DirectX;
  XMVECTOR Q0 = XMLoadFloat4(&q1);
  XMVECTOR Q1 = XMLoadFloat4(&q2);

  Quaternion result;
  XMStoreFloat4(&result, XMQuaternionMultiply(Q1, Q0));
  return result;
}

}  // namespace LL3D
}  // namespace Math

