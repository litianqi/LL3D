#pragma once

#include <functional>
#include <DirectXMath.h>
#include "Math/Vector.h"

namespace LL3D {
namespace Math {

using namespace DirectX;
struct Matrix;

//------------------------------------------------------------------------------
// Quaternion
struct Quaternion : public XMFLOAT4
{
  Quaternion() : XMFLOAT4(0, 0, 0, 1.f) {}
  Quaternion(float _x, float _y, float _z, float _w) : XMFLOAT4(_x, _y, _z, _w) {}
  Quaternion(const Vector3& v, float scalar) : XMFLOAT4(v.x, v.y, v.z, scalar) {}
  explicit Quaternion(const Vector4& v) : XMFLOAT4(v.x, v.y, v.z, v.w) {}
  explicit Quaternion(_In_reads_(4) const float *pArray) : XMFLOAT4(pArray) {}
  Quaternion(FXMVECTOR V) { XMStoreFloat4(this, V); }
  Quaternion(const XMFLOAT4& q) { this->x = q.x; this->y = q.y; this->z = q.z; this->w = q.w; }

  operator XMVECTOR() const { return XMLoadFloat4(this); }

  // Comparison operators
  bool operator == (const Quaternion& q) const;
  bool operator != (const Quaternion& q) const;

  // Assignment operators
  Quaternion& operator= (const Quaternion& q) { x = q.x; y = q.y; z = q.z; w = q.w; return *this; }
  Quaternion& operator= (const XMFLOAT4& q) { x = q.x; y = q.y; z = q.z; w = q.w; return *this; }
  Quaternion& operator+= (const Quaternion& q);
  Quaternion& operator-= (const Quaternion& q);
  Quaternion& operator*= (const Quaternion& q);
  Quaternion& operator*= (float S);
  Quaternion& operator/= (const Quaternion& q);

  // Unary operators
  Quaternion operator+ () const { return *this; }
  Quaternion operator- () const;

  // To PitchYawRoll Vector3
  Vector3 GetPitchYawRollVector()
  {
    return Vector3(GetPitch(), GetYaw(), GetRoll());
  }
  float GetYaw()
  {
    float x2 = x * x;
    float y2 = y * y;
    return (float)atan2(2.f * y * w - 2.f * z * x, 1.f - 2.f * y2 - 2.f * x2);
  }
  float GetPitch()
  {
    return (float)-asin(2.f * z * y + 2.f * x * w);
  }
  float GetRoll()
  {
    float x2 = x * x;
    float z2 = z * z;
    return (float)-atan2(2.f * z * w - 2.f * y * x, 1.f - 2.f * z2 - 2.f * x2);
  }

  // Quaternion operations
  float Length() const;
  float LengthSquared() const;

  void Normalize();
  void Normalize(Quaternion& result) const;

  void Conjugate();
  void Conjugate(Quaternion& result) const;

  void Inverse(Quaternion& result) const;

  float Dot(const Quaternion& Q) const;

  // Static functions
  static Quaternion CreateFromAxisAngle(const Vector3& axis, float angle);
  static Quaternion CreateFromYawPitchRoll(float yaw, float pitch, float roll);
  static Quaternion CreateFromRotationMatrix(const Matrix& M);

  static void Lerp(const Quaternion& q1, const Quaternion& q2, float t, Quaternion& result);
  static Quaternion Lerp(const Quaternion& q1, const Quaternion& q2, float t);

  static void Slerp(const Quaternion& q1, const Quaternion& q2, float t, Quaternion& result);
  static Quaternion Slerp(const Quaternion& q1, const Quaternion& q2, float t);

  static void Concatenate(const Quaternion& q1, const Quaternion& q2, Quaternion& result);
  static Quaternion Concatenate(const Quaternion& q1, const Quaternion& q2);

  // Constants
  static const Quaternion Identity;
};

// Binary operators
extern Quaternion operator+ (const Quaternion& Q1, const Quaternion& Q2);
extern Quaternion operator- (const Quaternion& Q1, const Quaternion& Q2);
extern Quaternion operator* (const Quaternion& Q1, const Quaternion& Q2);
extern Quaternion operator* (const Quaternion& Q, float S);
extern Quaternion operator/ (const Quaternion& Q1, const Quaternion& Q2);
extern Quaternion operator* (float S, const Quaternion& Q);

}  // namespace Math
}  // namespace LL3D

//------------------------------------------------------------------------------
// Support for Math and Standard C++ Library containers
namespace std {

template<> struct less<LL3D::Math::Quaternion>
{
  bool operator()(const LL3D::Math::Quaternion& Q1, const LL3D::Math::Quaternion& Q2) const
  {
    return ((Q1.x < Q2.x)
      || ((Q1.x == Q2.x) && (Q1.y < Q2.y))
      || ((Q1.x == Q2.x) && (Q1.y == Q2.y) && (Q1.z < Q2.z))
      || ((Q1.x == Q2.x) && (Q1.y == Q2.y) && (Q1.z == Q2.z) && (Q1.w < Q2.w)));
  }
};

}  // namespace std