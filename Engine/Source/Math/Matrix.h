#pragma once

#include <functional>
#include <DirectXMath.h>
#include "Math/Vector.h"

namespace LL3D {
namespace Math {

using namespace DirectX;
struct Plane;

//------------------------------------------------------------------------------
// 4x4 Matrix (assumes right-handed cooordinates)
struct Matrix : public XMFLOAT4X4
{
  Matrix() : XMFLOAT4X4(1.f, 0, 0, 0,
    0, 1.f, 0, 0,
    0, 0, 1.f, 0,
    0, 0, 0, 1.f) {}
  Matrix(float m00, float m01, float m02, float m03,
    float m10, float m11, float m12, float m13,
    float m20, float m21, float m22, float m23,
    float m30, float m31, float m32, float m33) : XMFLOAT4X4(m00, m01, m02, m03,
      m10, m11, m12, m13,
      m20, m21, m22, m23,
      m30, m31, m32, m33) {}
  explicit Matrix(const Vector3& r0, const Vector3& r1, const Vector3& r2) : XMFLOAT4X4(r0.x, r0.y, r0.z, 0,
    r1.x, r1.y, r1.z, 0,
    r2.x, r2.y, r2.z, 0,
    0, 0, 0, 1.f) {}
  explicit Matrix(const Vector4& r0, const Vector4& r1, const Vector4& r2, const Vector4& r3) : XMFLOAT4X4(r0.x, r0.y, r0.z, r0.w,
    r1.x, r1.y, r1.z, r1.w,
    r2.x, r2.y, r2.z, r2.w,
    r3.x, r3.y, r3.z, r3.w) {}
  Matrix(const XMFLOAT4X4& M) { memcpy_s(this, sizeof(float) * 16, &M, sizeof(XMFLOAT4X4)); }
  Matrix(const XMFLOAT3X3& M);
  Matrix(const XMFLOAT4X3& M);

  explicit Matrix(_In_reads_(16) const float *pArray) : XMFLOAT4X4(pArray) {}
  Matrix(CXMMATRIX M) { XMStoreFloat4x4(this, M); }

  operator XMMATRIX() const { return XMLoadFloat4x4(this); }

  // Comparison operators
  inline bool operator == (const Matrix& M) const;
  bool operator != (const Matrix& M) const;

  // Assignment operators
  Matrix& operator= (const Matrix& M) { memcpy_s(this, sizeof(float) * 16, &M, sizeof(float) * 16); return *this; }
  Matrix& operator= (const XMFLOAT4X4& M) { memcpy_s(this, sizeof(float) * 16, &M, sizeof(XMFLOAT4X4)); return *this; }
  Matrix& operator= (const XMFLOAT3X3& M);
  Matrix& operator= (const XMFLOAT4X3& M);
  Matrix& operator+= (const Matrix& M);
  Matrix& operator-= (const Matrix& M);
  Matrix& operator*= (const Matrix& M);
  Matrix& operator*= (float S);
  Matrix& operator/= (float S);

  Matrix& operator/= (const Matrix& M);
  // Element-wise divide

  // Unary operators
  Matrix operator+ () const { return *this; }
  Matrix operator- () const;

  // Properties
  Vector3 Up() const { return Vector3(_21, _22, _23); }
  void Up(const Vector3& v) { _21 = v.x; _22 = v.y; _23 = v.z; }

  Vector3 Down() const { return Vector3(-_21, -_22, -_23); }
  void Down(const Vector3& v) { _21 = -v.x; _22 = -v.y; _23 = -v.z; }

  Vector3 Right() const { return Vector3(_11, _12, _13); }
  void Right(const Vector3& v) { _11 = v.x; _12 = v.y; _13 = v.z; }

  Vector3 Left() const { return Vector3(-_11, -_12, -_13); }
  void Left(const Vector3& v) { _11 = -v.x; _12 = -v.y; _13 = -v.z; }

  Vector3 Forward() const { return Vector3(-_31, -_32, -_33); }
  void Forward(const Vector3& v) { _31 = -v.x; _32 = -v.y; _33 = -v.z; }

  Vector3 Backward() const { return Vector3(_31, _32, _33); }
  void Backward(const Vector3& v) { _31 = v.x; _32 = v.y; _33 = v.z; }

  Vector3 Translation() const { return Vector3(_41, _42, _43); }
  void Translation(const Vector3& v) { _41 = v.x; _42 = v.y; _43 = v.z; }

  // Matrix operations
  bool Decompose(Vector3& scale, Quaternion& rotation, Vector3& translation);

  Matrix Transpose() const;
  void Transpose(Matrix& result) const;

  Matrix Invert() const;
  void Invert(Matrix& result) const;

  float Determinant() const;

  // Static functions
  static Matrix CreateBillboard(const Vector3& object, const Vector3& cameraPosition, const Vector3& cameraUp, _In_opt_ const Vector3* cameraForward = nullptr);

  static Matrix CreateConstrainedBillboard(const Vector3& object, const Vector3& cameraPosition, const Vector3& rotateAxis,
    _In_opt_ const Vector3* cameraForward = nullptr, _In_opt_ const Vector3* objectForward = nullptr);

  static Matrix CreateTranslation(const Vector3& position);
  static Matrix CreateTranslation(float x, float y, float z);

  static Matrix CreateScale(const Vector3& scales);
  static Matrix CreateScale(float xs, float ys, float zs);
  static Matrix CreateScale(float scale);

  static Matrix CreateRotationX(float radians);
  static Matrix CreateRotationY(float radians);
  static Matrix CreateRotationZ(float radians);

  static Matrix CreateFromAxisAngle(const Vector3& axis, float angle);

  static Matrix CreatePerspectiveFieldOfView(float fov, float aspectRatio, float nearPlane, float farPlane);
  static Matrix CreatePerspective(float width, float height, float nearPlane, float farPlane);
  static Matrix CreatePerspectiveOffCenter(float left, float right, float bottom, float top, float nearPlane, float farPlane);
  static Matrix CreateOrthographic(float width, float height, float zNearPlane, float zFarPlane);
  static Matrix CreateOrthographicOffCenter(float left, float right, float bottom, float top, float zNearPlane, float zFarPlane);

  static Matrix CreateLookAt(const Vector3& camera_pos, const Vector3& target_pos, const Vector3& up_vec);
  static Matrix CreateLookTo(const Vector3& camera_pos, const Vector3& target_vec, const Vector3& up_vec);
  static Matrix CreateWorld(const Vector3& position, const Vector3& forward, const Vector3& up);

  static Matrix CreateFromQuaternion(const Quaternion& quat);

  static Matrix CreateFromYawPitchRoll(float yaw, float pitch, float roll);

  static Matrix CreateShadow(const Vector3& lightDir, const Plane& plane);
  static Matrix CreateShadow(const Vector4& light, const Plane& plane);

  static Matrix CreateReflection(const Plane& plane);

  static void Lerp(const Matrix& M1, const Matrix& M2, float t, Matrix& result);
  static Matrix Lerp(const Matrix& M1, const Matrix& M2, float t);

  static void Transform(const Matrix& M, const Quaternion& rotation, Matrix& result);
  static Matrix Transform(const Matrix& M, const Quaternion& rotation);

  // Constants
  static const Matrix Identity;
};

// Binary operators
extern Matrix operator+ (const Matrix& M1, const Matrix& M2);
extern Matrix operator- (const Matrix& M1, const Matrix& M2);
extern Matrix operator* (const Matrix& M1, const Matrix& M2);
extern Matrix operator* (const Matrix& M, float S);
extern Matrix operator/ (const Matrix& M, float S);
extern Matrix operator/ (const Matrix& M1, const Matrix& M2);
// Element-wise divide
extern Matrix operator* (float S, const Matrix& M);

}  // namespace Math
}  // namespace LL3D

//------------------------------------------------------------------------------
// Support for Math and Standard C++ Library containers
namespace std {

template<> struct less<LL3D::Math::Matrix>
{
  bool operator()(const LL3D::Math::Matrix& M1, const LL3D::Math::Matrix& M2) const
  {
    if (M1._11 != M2._11) return M1._11 < M2._11;
    if (M1._12 != M2._12) return M1._12 < M2._12;
    if (M1._13 != M2._13) return M1._13 < M2._13;
    if (M1._14 != M2._14) return M1._14 < M2._14;
    if (M1._21 != M2._21) return M1._21 < M2._21;
    if (M1._22 != M2._22) return M1._22 < M2._22;
    if (M1._23 != M2._23) return M1._23 < M2._23;
    if (M1._24 != M2._24) return M1._24 < M2._24;
    if (M1._31 != M2._31) return M1._31 < M2._31;
    if (M1._32 != M2._32) return M1._32 < M2._32;
    if (M1._33 != M2._33) return M1._33 < M2._33;
    if (M1._34 != M2._34) return M1._34 < M2._34;
    if (M1._41 != M2._41) return M1._41 < M2._41;
    if (M1._42 != M2._42) return M1._42 < M2._42;
    if (M1._43 != M2._43) return M1._43 < M2._43;
    if (M1._44 != M2._44) return M1._44 < M2._44;

    return false;
  }
};

}  // namespace std