#include "Math/Vector.h"
#include "Math/Quaternion.h"
#include "Math/Matrix.h"

namespace LL3D {
namespace Math {

/****************************************************************************
*
* Vector2
*
****************************************************************************/

//------------------------------------------------------------------------------
// Comparision operators
//------------------------------------------------------------------------------

bool Vector2::operator == (const Vector2& V) const
{
  using namespace DirectX;
  XMVECTOR v1 = XMLoadFloat2(this);
  XMVECTOR v2 = XMLoadFloat2(&V);
  return XMVector2Equal(v1, v2);
}

bool Vector2::operator != (const Vector2& V) const
{
  using namespace DirectX;
  XMVECTOR v1 = XMLoadFloat2(this);
  XMVECTOR v2 = XMLoadFloat2(&V);
  return XMVector2NotEqual(v1, v2);
}

//------------------------------------------------------------------------------
// Assignment operators
//------------------------------------------------------------------------------

Vector2& Vector2::operator+= (const Vector2& V)
{
  using namespace DirectX;
  XMVECTOR v1 = XMLoadFloat2(this);
  XMVECTOR v2 = XMLoadFloat2(&V);
  XMVECTOR X = XMVectorAdd(v1, v2);
  XMStoreFloat2(this, X);
  return *this;
}

Vector2& Vector2::operator-= (const Vector2& V)
{
  using namespace DirectX;
  XMVECTOR v1 = XMLoadFloat2(this);
  XMVECTOR v2 = XMLoadFloat2(&V);
  XMVECTOR X = XMVectorSubtract(v1, v2);
  XMStoreFloat2(this, X);
  return *this;
}

Vector2& Vector2::operator*= (const Vector2& V)
{
  using namespace DirectX;
  XMVECTOR v1 = XMLoadFloat2(this);
  XMVECTOR v2 = XMLoadFloat2(&V);
  XMVECTOR X = XMVectorMultiply(v1, v2);
  XMStoreFloat2(this, X);
  return *this;
}

Vector2& Vector2::operator*= (float S)
{
  using namespace DirectX;
  XMVECTOR v1 = XMLoadFloat2(this);
  XMVECTOR X = XMVectorScale(v1, S);
  XMStoreFloat2(this, X);
  return *this;
}

Vector2& Vector2::operator/= (float S)
{
  using namespace DirectX;
  assert(S != 0.0f);
  XMVECTOR v1 = XMLoadFloat2(this);
  XMVECTOR X = XMVectorScale(v1, 1.f / S);
  XMStoreFloat2(this, X);
  return *this;
}

//------------------------------------------------------------------------------
// Binary operators
//------------------------------------------------------------------------------

Vector2 operator+ (const Vector2& V1, const Vector2& V2)
{
  using namespace DirectX;
  XMVECTOR v1 = XMLoadFloat2(&V1);
  XMVECTOR v2 = XMLoadFloat2(&V2);
  XMVECTOR X = XMVectorAdd(v1, v2);
  Vector2 R;
  XMStoreFloat2(&R, X);
  return R;
}

Vector2 operator- (const Vector2& V1, const Vector2& V2)
{
  using namespace DirectX;
  XMVECTOR v1 = XMLoadFloat2(&V1);
  XMVECTOR v2 = XMLoadFloat2(&V2);
  XMVECTOR X = XMVectorSubtract(v1, v2);
  Vector2 R;
  XMStoreFloat2(&R, X);
  return R;
}

Vector2 operator* (const Vector2& V1, const Vector2& V2)
{
  using namespace DirectX;
  XMVECTOR v1 = XMLoadFloat2(&V1);
  XMVECTOR v2 = XMLoadFloat2(&V2);
  XMVECTOR X = XMVectorMultiply(v1, v2);
  Vector2 R;
  XMStoreFloat2(&R, X);
  return R;
}

Vector2 operator* (const Vector2& V, float S)
{
  using namespace DirectX;
  XMVECTOR v1 = XMLoadFloat2(&V);
  XMVECTOR X = XMVectorScale(v1, S);
  Vector2 R;
  XMStoreFloat2(&R, X);
  return R;
}

Vector2 operator/ (const Vector2& V1, const Vector2& V2)
{
  using namespace DirectX;
  XMVECTOR v1 = XMLoadFloat2(&V1);
  XMVECTOR v2 = XMLoadFloat2(&V2);
  XMVECTOR X = XMVectorDivide(v1, v2);
  Vector2 R;
  XMStoreFloat2(&R, X);
  return R;
}

Vector2 operator* (float S, const Vector2& V)
{
  using namespace DirectX;
  XMVECTOR v1 = XMLoadFloat2(&V);
  XMVECTOR X = XMVectorScale(v1, S);
  Vector2 R;
  XMStoreFloat2(&R, X);
  return R;
}

//------------------------------------------------------------------------------
// Vector operations
//------------------------------------------------------------------------------

bool Vector2::InBounds(const Vector2& Bounds) const
{
  using namespace DirectX;
  XMVECTOR v1 = XMLoadFloat2(this);
  XMVECTOR v2 = XMLoadFloat2(&Bounds);
  return XMVector2InBounds(v1, v2);
}

float Vector2::Length() const
{
  using namespace DirectX;
  XMVECTOR v1 = XMLoadFloat2(this);
  XMVECTOR X = XMVector2Length(v1);
  return XMVectorGetX(X);
}

float Vector2::LengthSquared() const
{
  using namespace DirectX;
  XMVECTOR v1 = XMLoadFloat2(this);
  XMVECTOR X = XMVector2LengthSq(v1);
  return XMVectorGetX(X);
}

float Vector2::Dot(const Vector2& V) const
{
  using namespace DirectX;
  XMVECTOR v1 = XMLoadFloat2(this);
  XMVECTOR v2 = XMLoadFloat2(&V);
  XMVECTOR X = XMVector2Dot(v1, v2);
  return XMVectorGetX(X);
}

void Vector2::Cross(const Vector2& V, Vector2& result) const
{
  using namespace DirectX;
  XMVECTOR v1 = XMLoadFloat2(this);
  XMVECTOR v2 = XMLoadFloat2(&V);
  XMVECTOR R = XMVector2Cross(v1, v2);
  XMStoreFloat2(&result, R);
}

Vector2 Vector2::Cross(const Vector2& V) const
{
  using namespace DirectX;
  XMVECTOR v1 = XMLoadFloat2(this);
  XMVECTOR v2 = XMLoadFloat2(&V);
  XMVECTOR R = XMVector2Cross(v1, v2);

  Vector2 result;
  XMStoreFloat2(&result, R);
  return result;
}

void Vector2::Normalize()
{
  using namespace DirectX;
  XMVECTOR v1 = XMLoadFloat2(this);
  XMVECTOR X = XMVector2Normalize(v1);
  XMStoreFloat2(this, X);
}

void Vector2::Normalize(Vector2& result) const
{
  using namespace DirectX;
  XMVECTOR v1 = XMLoadFloat2(this);
  XMVECTOR X = XMVector2Normalize(v1);
  XMStoreFloat2(&result, X);
}

void Vector2::Clamp(const Vector2& vmin, const Vector2& vmax)
{
  using namespace DirectX;
  XMVECTOR v1 = XMLoadFloat2(this);
  XMVECTOR v2 = XMLoadFloat2(&vmin);
  XMVECTOR v3 = XMLoadFloat2(&vmax);
  XMVECTOR X = XMVectorClamp(v1, v2, v3);
  XMStoreFloat2(this, X);
}

void Vector2::Clamp(const Vector2& vmin, const Vector2& vmax, Vector2& result) const
{
  using namespace DirectX;
  XMVECTOR v1 = XMLoadFloat2(this);
  XMVECTOR v2 = XMLoadFloat2(&vmin);
  XMVECTOR v3 = XMLoadFloat2(&vmax);
  XMVECTOR X = XMVectorClamp(v1, v2, v3);
  XMStoreFloat2(&result, X);
}

//------------------------------------------------------------------------------
// Static functions
//------------------------------------------------------------------------------

float Vector2::Distance(const Vector2& v1, const Vector2& v2)
{
  using namespace DirectX;
  XMVECTOR x1 = XMLoadFloat2(&v1);
  XMVECTOR x2 = XMLoadFloat2(&v2);
  XMVECTOR V = XMVectorSubtract(x2, x1);
  XMVECTOR X = XMVector2Length(V);
  return XMVectorGetX(X);
}

float Vector2::DistanceSquared(const Vector2& v1, const Vector2& v2)
{
  using namespace DirectX;
  XMVECTOR x1 = XMLoadFloat2(&v1);
  XMVECTOR x2 = XMLoadFloat2(&v2);
  XMVECTOR V = XMVectorSubtract(x2, x1);
  XMVECTOR X = XMVector2LengthSq(V);
  return XMVectorGetX(X);
}

void Vector2::Min(const Vector2& v1, const Vector2& v2, Vector2& result)
{
  using namespace DirectX;
  XMVECTOR x1 = XMLoadFloat2(&v1);
  XMVECTOR x2 = XMLoadFloat2(&v2);
  XMVECTOR X = XMVectorMin(x1, x2);
  XMStoreFloat2(&result, X);
}

Vector2 Vector2::Min(const Vector2& v1, const Vector2& v2)
{
  using namespace DirectX;
  XMVECTOR x1 = XMLoadFloat2(&v1);
  XMVECTOR x2 = XMLoadFloat2(&v2);
  XMVECTOR X = XMVectorMin(x1, x2);

  Vector2 result;
  XMStoreFloat2(&result, X);
  return result;
}

void Vector2::Max(const Vector2& v1, const Vector2& v2, Vector2& result)
{
  using namespace DirectX;
  XMVECTOR x1 = XMLoadFloat2(&v1);
  XMVECTOR x2 = XMLoadFloat2(&v2);
  XMVECTOR X = XMVectorMax(x1, x2);
  XMStoreFloat2(&result, X);
}

Vector2 Vector2::Max(const Vector2& v1, const Vector2& v2)
{
  using namespace DirectX;
  XMVECTOR x1 = XMLoadFloat2(&v1);
  XMVECTOR x2 = XMLoadFloat2(&v2);
  XMVECTOR X = XMVectorMax(x1, x2);

  Vector2 result;
  XMStoreFloat2(&result, X);
  return result;
}

void Vector2::Lerp(const Vector2& v1, const Vector2& v2, float t, Vector2& result)
{
  using namespace DirectX;
  XMVECTOR x1 = XMLoadFloat2(&v1);
  XMVECTOR x2 = XMLoadFloat2(&v2);
  XMVECTOR X = XMVectorLerp(x1, x2, t);
  XMStoreFloat2(&result, X);
}

Vector2 Vector2::Lerp(const Vector2& v1, const Vector2& v2, float t)
{
  using namespace DirectX;
  XMVECTOR x1 = XMLoadFloat2(&v1);
  XMVECTOR x2 = XMLoadFloat2(&v2);
  XMVECTOR X = XMVectorLerp(x1, x2, t);

  Vector2 result;
  XMStoreFloat2(&result, X);
  return result;
}

void Vector2::SmoothStep(const Vector2& v1, const Vector2& v2, float t, Vector2& result)
{
  using namespace DirectX;
  t = (t > 1.0f) ? 1.0f : ((t < 0.0f) ? 0.0f : t);  // Clamp value to 0 to 1
  t = t*t*(3.f - 2.f*t);
  XMVECTOR x1 = XMLoadFloat2(&v1);
  XMVECTOR x2 = XMLoadFloat2(&v2);
  XMVECTOR X = XMVectorLerp(x1, x2, t);
  XMStoreFloat2(&result, X);
}

Vector2 Vector2::SmoothStep(const Vector2& v1, const Vector2& v2, float t)
{
  using namespace DirectX;
  t = (t > 1.0f) ? 1.0f : ((t < 0.0f) ? 0.0f : t);  // Clamp value to 0 to 1
  t = t*t*(3.f - 2.f*t);
  XMVECTOR x1 = XMLoadFloat2(&v1);
  XMVECTOR x2 = XMLoadFloat2(&v2);
  XMVECTOR X = XMVectorLerp(x1, x2, t);

  Vector2 result;
  XMStoreFloat2(&result, X);
  return result;
}

void Vector2::Barycentric(const Vector2& v1, const Vector2& v2, const Vector2& v3, float f, float g, Vector2& result)
{
  using namespace DirectX;
  XMVECTOR x1 = XMLoadFloat2(&v1);
  XMVECTOR x2 = XMLoadFloat2(&v2);
  XMVECTOR x3 = XMLoadFloat2(&v3);
  XMVECTOR X = XMVectorBaryCentric(x1, x2, x3, f, g);
  XMStoreFloat2(&result, X);
}

Vector2 Vector2::Barycentric(const Vector2& v1, const Vector2& v2, const Vector2& v3, float f, float g)
{
  using namespace DirectX;
  XMVECTOR x1 = XMLoadFloat2(&v1);
  XMVECTOR x2 = XMLoadFloat2(&v2);
  XMVECTOR x3 = XMLoadFloat2(&v3);
  XMVECTOR X = XMVectorBaryCentric(x1, x2, x3, f, g);

  Vector2 result;
  XMStoreFloat2(&result, X);
  return result;
}

void Vector2::CatmullRom(const Vector2& v1, const Vector2& v2, const Vector2& v3, const Vector2& v4, float t, Vector2& result)
{
  using namespace DirectX;
  XMVECTOR x1 = XMLoadFloat2(&v1);
  XMVECTOR x2 = XMLoadFloat2(&v2);
  XMVECTOR x3 = XMLoadFloat2(&v3);
  XMVECTOR x4 = XMLoadFloat2(&v4);
  XMVECTOR X = XMVectorCatmullRom(x1, x2, x3, x4, t);
  XMStoreFloat2(&result, X);
}

Vector2 Vector2::CatmullRom(const Vector2& v1, const Vector2& v2, const Vector2& v3, const Vector2& v4, float t)
{
  using namespace DirectX;
  XMVECTOR x1 = XMLoadFloat2(&v1);
  XMVECTOR x2 = XMLoadFloat2(&v2);
  XMVECTOR x3 = XMLoadFloat2(&v3);
  XMVECTOR x4 = XMLoadFloat2(&v4);
  XMVECTOR X = XMVectorCatmullRom(x1, x2, x3, x4, t);

  Vector2 result;
  XMStoreFloat2(&result, X);
  return result;
}

void Vector2::Hermite(const Vector2& v1, const Vector2& t1, const Vector2& v2, const Vector2& t2, float t, Vector2& result)
{
  using namespace DirectX;
  XMVECTOR x1 = XMLoadFloat2(&v1);
  XMVECTOR x2 = XMLoadFloat2(&t1);
  XMVECTOR x3 = XMLoadFloat2(&v2);
  XMVECTOR x4 = XMLoadFloat2(&t2);
  XMVECTOR X = XMVectorHermite(x1, x2, x3, x4, t);
  XMStoreFloat2(&result, X);
}

Vector2 Vector2::Hermite(const Vector2& v1, const Vector2& t1, const Vector2& v2, const Vector2& t2, float t)
{
  using namespace DirectX;
  XMVECTOR x1 = XMLoadFloat2(&v1);
  XMVECTOR x2 = XMLoadFloat2(&t1);
  XMVECTOR x3 = XMLoadFloat2(&v2);
  XMVECTOR x4 = XMLoadFloat2(&t2);
  XMVECTOR X = XMVectorHermite(x1, x2, x3, x4, t);

  Vector2 result;
  XMStoreFloat2(&result, X);
  return result;
}

void Vector2::Reflect(const Vector2& ivec, const Vector2& nvec, Vector2& result)
{
  using namespace DirectX;
  XMVECTOR i = XMLoadFloat2(&ivec);
  XMVECTOR n = XMLoadFloat2(&nvec);
  XMVECTOR X = XMVector2Reflect(i, n);
  XMStoreFloat2(&result, X);
}

Vector2 Vector2::Reflect(const Vector2& ivec, const Vector2& nvec)
{
  using namespace DirectX;
  XMVECTOR i = XMLoadFloat2(&ivec);
  XMVECTOR n = XMLoadFloat2(&nvec);
  XMVECTOR X = XMVector2Reflect(i, n);

  Vector2 result;
  XMStoreFloat2(&result, X);
  return result;
}

void Vector2::Refract(const Vector2& ivec, const Vector2& nvec, float refractionIndex, Vector2& result)
{
  using namespace DirectX;
  XMVECTOR i = XMLoadFloat2(&ivec);
  XMVECTOR n = XMLoadFloat2(&nvec);
  XMVECTOR X = XMVector2Refract(i, n, refractionIndex);
  XMStoreFloat2(&result, X);
}

Vector2 Vector2::Refract(const Vector2& ivec, const Vector2& nvec, float refractionIndex)
{
  using namespace DirectX;
  XMVECTOR i = XMLoadFloat2(&ivec);
  XMVECTOR n = XMLoadFloat2(&nvec);
  XMVECTOR X = XMVector2Refract(i, n, refractionIndex);

  Vector2 result;
  XMStoreFloat2(&result, X);
  return result;
}

void Vector2::Transform(const Vector2& v, const Quaternion& quat, Vector2& result)
{
  using namespace DirectX;
  XMVECTOR v1 = XMLoadFloat2(&v);
  XMVECTOR q = XMLoadFloat4(&quat);
  XMVECTOR X = XMVector3Rotate(v1, q);
  XMStoreFloat2(&result, X);
}

Vector2 Vector2::Transform(const Vector2& v, const Quaternion& quat)
{
  using namespace DirectX;
  XMVECTOR v1 = XMLoadFloat2(&v);
  XMVECTOR q = XMLoadFloat4(&quat);
  XMVECTOR X = XMVector3Rotate(v1, q);

  Vector2 result;
  XMStoreFloat2(&result, X);
  return result;
}

void Vector2::Transform(const Vector2& v, const Matrix& m, Vector2& result)
{
  using namespace DirectX;
  XMVECTOR v1 = XMLoadFloat2(&v);
  XMMATRIX M = XMLoadFloat4x4(&m);
  XMVECTOR X = XMVector2TransformCoord(v1, M);
  XMStoreFloat2(&result, X);
}

Vector2 Vector2::Transform(const Vector2& v, const Matrix& m)
{
  using namespace DirectX;
  XMVECTOR v1 = XMLoadFloat2(&v);
  XMMATRIX M = XMLoadFloat4x4(&m);
  XMVECTOR X = XMVector2TransformCoord(v1, M);

  Vector2 result;
  XMStoreFloat2(&result, X);
  return result;
}

_Use_decl_annotations_
void Vector2::Transform(const Vector2* varray, size_t count, const Matrix& m, Vector2* resultArray)
{
  using namespace DirectX;
  XMMATRIX M = XMLoadFloat4x4(&m);
  XMVector2TransformCoordStream(resultArray, sizeof(XMFLOAT2), varray, sizeof(XMFLOAT2), count, M);
}

void Vector2::Transform(const Vector2& v, const Matrix& m, Vector4& result)
{
  using namespace DirectX;
  XMVECTOR v1 = XMLoadFloat2(&v);
  XMMATRIX M = XMLoadFloat4x4(&m);
  XMVECTOR X = XMVector2Transform(v1, M);
  XMStoreFloat4(&result, X);
}

_Use_decl_annotations_
void Vector2::Transform(const Vector2* varray, size_t count, const Matrix& m, Vector4* resultArray)
{
  using namespace DirectX;
  XMMATRIX M = XMLoadFloat4x4(&m);
  XMVector2TransformStream(resultArray, sizeof(XMFLOAT4), varray, sizeof(XMFLOAT2), count, M);
}

void Vector2::TransformNormal(const Vector2& v, const Matrix& m, Vector2& result)
{
  using namespace DirectX;
  XMVECTOR v1 = XMLoadFloat2(&v);
  XMMATRIX M = XMLoadFloat4x4(&m);
  XMVECTOR X = XMVector2TransformNormal(v1, M);
  XMStoreFloat2(&result, X);
}

Vector2 Vector2::TransformNormal(const Vector2& v, const Matrix& m)
{
  using namespace DirectX;
  XMVECTOR v1 = XMLoadFloat2(&v);
  XMMATRIX M = XMLoadFloat4x4(&m);
  XMVECTOR X = XMVector2TransformNormal(v1, M);

  Vector2 result;
  XMStoreFloat2(&result, X);
  return result;
}

_Use_decl_annotations_
void Vector2::TransformNormal(const Vector2* varray, size_t count, const Matrix& m, Vector2* resultArray)
{
  using namespace DirectX;
  XMMATRIX M = XMLoadFloat4x4(&m);
  XMVector2TransformNormalStream(resultArray, sizeof(XMFLOAT2), varray, sizeof(XMFLOAT2), count, M);
}


/****************************************************************************
*
* Vector3
*
****************************************************************************/

//------------------------------------------------------------------------------
// Comparision operators
//------------------------------------------------------------------------------

bool Vector3::operator == (const Vector3& V) const
{
  using namespace DirectX;
  XMVECTOR v1 = XMLoadFloat3(this);
  XMVECTOR v2 = XMLoadFloat3(&V);
  return XMVector3Equal(v1, v2);
}

bool Vector3::operator != (const Vector3& V) const
{
  using namespace DirectX;
  XMVECTOR v1 = XMLoadFloat3(this);
  XMVECTOR v2 = XMLoadFloat3(&V);
  return XMVector3NotEqual(v1, v2);
}

//------------------------------------------------------------------------------
// Assignment operators
//------------------------------------------------------------------------------

Vector3& Vector3::operator+= (const Vector3& V)
{
  using namespace DirectX;
  XMVECTOR v1 = XMLoadFloat3(this);
  XMVECTOR v2 = XMLoadFloat3(&V);
  XMVECTOR X = XMVectorAdd(v1, v2);
  XMStoreFloat3(this, X);
  return *this;
}

Vector3& Vector3::operator-= (const Vector3& V)
{
  using namespace DirectX;
  XMVECTOR v1 = XMLoadFloat3(this);
  XMVECTOR v2 = XMLoadFloat3(&V);
  XMVECTOR X = XMVectorSubtract(v1, v2);
  XMStoreFloat3(this, X);
  return *this;
}

Vector3& Vector3::operator*= (const Vector3& V)
{
  using namespace DirectX;
  XMVECTOR v1 = XMLoadFloat3(this);
  XMVECTOR v2 = XMLoadFloat3(&V);
  XMVECTOR X = XMVectorMultiply(v1, v2);
  XMStoreFloat3(this, X);
  return *this;
}

Vector3& Vector3::operator*= (float S)
{
  using namespace DirectX;
  XMVECTOR v1 = XMLoadFloat3(this);
  XMVECTOR X = XMVectorScale(v1, S);
  XMStoreFloat3(this, X);
  return *this;
}

Vector3& Vector3::operator/= (float S)
{
  using namespace DirectX;
  assert(S != 0.0f);
  XMVECTOR v1 = XMLoadFloat3(this);
  XMVECTOR X = XMVectorScale(v1, 1.f / S);
  XMStoreFloat3(this, X);
  return *this;
}

//------------------------------------------------------------------------------
// Urnary operators
//------------------------------------------------------------------------------

Vector3 Vector3::operator- () const
{
  using namespace DirectX;
  XMVECTOR v1 = XMLoadFloat3(this);
  XMVECTOR X = XMVectorNegate(v1);
  Vector3 R;
  XMStoreFloat3(&R, X);
  return R;
}

//------------------------------------------------------------------------------
// Binary operators
//------------------------------------------------------------------------------

Vector3 operator+ (const Vector3& V1, const Vector3& V2)
{
  using namespace DirectX;
  XMVECTOR v1 = XMLoadFloat3(&V1);
  XMVECTOR v2 = XMLoadFloat3(&V2);
  XMVECTOR X = XMVectorAdd(v1, v2);
  Vector3 R;
  XMStoreFloat3(&R, X);
  return R;
}

Vector3 operator- (const Vector3& V1, const Vector3& V2)
{
  using namespace DirectX;
  XMVECTOR v1 = XMLoadFloat3(&V1);
  XMVECTOR v2 = XMLoadFloat3(&V2);
  XMVECTOR X = XMVectorSubtract(v1, v2);
  Vector3 R;
  XMStoreFloat3(&R, X);
  return R;
}

Vector3 operator* (const Vector3& V1, const Vector3& V2)
{
  using namespace DirectX;
  XMVECTOR v1 = XMLoadFloat3(&V1);
  XMVECTOR v2 = XMLoadFloat3(&V2);
  XMVECTOR X = XMVectorMultiply(v1, v2);
  Vector3 R;
  XMStoreFloat3(&R, X);
  return R;
}

Vector3 operator* (const Vector3& V, float S)
{
  using namespace DirectX;
  XMVECTOR v1 = XMLoadFloat3(&V);
  XMVECTOR X = XMVectorScale(v1, S);
  Vector3 R;
  XMStoreFloat3(&R, X);
  return R;
}

Vector3 operator/ (const Vector3& V1, const Vector3& V2)
{
  using namespace DirectX;
  XMVECTOR v1 = XMLoadFloat3(&V1);
  XMVECTOR v2 = XMLoadFloat3(&V2);
  XMVECTOR X = XMVectorDivide(v1, v2);
  Vector3 R;
  XMStoreFloat3(&R, X);
  return R;
}

Vector3 operator* (float S, const Vector3& V)
{
  using namespace DirectX;
  XMVECTOR v1 = XMLoadFloat3(&V);
  XMVECTOR X = XMVectorScale(v1, S);
  Vector3 R;
  XMStoreFloat3(&R, X);
  return R;
}

//------------------------------------------------------------------------------
// Vector operations
//------------------------------------------------------------------------------

bool Vector3::InBounds(const Vector3& Bounds) const
{
  using namespace DirectX;
  XMVECTOR v1 = XMLoadFloat3(this);
  XMVECTOR v2 = XMLoadFloat3(&Bounds);
  return XMVector3InBounds(v1, v2);
}

float Vector3::Length() const
{
  using namespace DirectX;
  XMVECTOR v1 = XMLoadFloat3(this);
  XMVECTOR X = XMVector3Length(v1);
  return XMVectorGetX(X);
}

float Vector3::LengthSquared() const
{
  using namespace DirectX;
  XMVECTOR v1 = XMLoadFloat3(this);
  XMVECTOR X = XMVector3LengthSq(v1);
  return XMVectorGetX(X);
}

float Vector3::Dot(const Vector3& V) const
{
  using namespace DirectX;
  XMVECTOR v1 = XMLoadFloat3(this);
  XMVECTOR v2 = XMLoadFloat3(&V);
  XMVECTOR X = XMVector3Dot(v1, v2);
  return XMVectorGetX(X);
}

void Vector3::Cross(const Vector3& V, Vector3& result) const
{
  using namespace DirectX;
  XMVECTOR v1 = XMLoadFloat3(this);
  XMVECTOR v2 = XMLoadFloat3(&V);
  XMVECTOR R = XMVector3Cross(v1, v2);
  XMStoreFloat3(&result, R);
}

Vector3 Vector3::Cross(const Vector3& V) const
{
  using namespace DirectX;
  XMVECTOR v1 = XMLoadFloat3(this);
  XMVECTOR v2 = XMLoadFloat3(&V);
  XMVECTOR R = XMVector3Cross(v1, v2);

  Vector3 result;
  XMStoreFloat3(&result, R);
  return result;
}

void Vector3::Normalize()
{
  using namespace DirectX;
  XMVECTOR v1 = XMLoadFloat3(this);
  XMVECTOR X = XMVector3Normalize(v1);
  XMStoreFloat3(this, X);
}

void Vector3::Normalize(Vector3& result) const
{
  using namespace DirectX;
  XMVECTOR v1 = XMLoadFloat3(this);
  XMVECTOR X = XMVector3Normalize(v1);
  XMStoreFloat3(&result, X);
}

void Vector3::Clamp(const Vector3& vmin, const Vector3& vmax)
{
  using namespace DirectX;
  XMVECTOR v1 = XMLoadFloat3(this);
  XMVECTOR v2 = XMLoadFloat3(&vmin);
  XMVECTOR v3 = XMLoadFloat3(&vmax);
  XMVECTOR X = XMVectorClamp(v1, v2, v3);
  XMStoreFloat3(this, X);
}

void Vector3::Clamp(const Vector3& vmin, const Vector3& vmax, Vector3& result) const
{
  using namespace DirectX;
  XMVECTOR v1 = XMLoadFloat3(this);
  XMVECTOR v2 = XMLoadFloat3(&vmin);
  XMVECTOR v3 = XMLoadFloat3(&vmax);
  XMVECTOR X = XMVectorClamp(v1, v2, v3);
  XMStoreFloat3(&result, X);
}

//------------------------------------------------------------------------------
// Static functions
//------------------------------------------------------------------------------

float Vector3::Distance(const Vector3& v1, const Vector3& v2)
{
  using namespace DirectX;
  XMVECTOR x1 = XMLoadFloat3(&v1);
  XMVECTOR x2 = XMLoadFloat3(&v2);
  XMVECTOR V = XMVectorSubtract(x2, x1);
  XMVECTOR X = XMVector3Length(V);
  return XMVectorGetX(X);
}

float Vector3::DistanceSquared(const Vector3& v1, const Vector3& v2)
{
  using namespace DirectX;
  XMVECTOR x1 = XMLoadFloat3(&v1);
  XMVECTOR x2 = XMLoadFloat3(&v2);
  XMVECTOR V = XMVectorSubtract(x2, x1);
  XMVECTOR X = XMVector3LengthSq(V);
  return XMVectorGetX(X);
}

void Vector3::Min(const Vector3& v1, const Vector3& v2, Vector3& result)
{
  using namespace DirectX;
  XMVECTOR x1 = XMLoadFloat3(&v1);
  XMVECTOR x2 = XMLoadFloat3(&v2);
  XMVECTOR X = XMVectorMin(x1, x2);
  XMStoreFloat3(&result, X);
}

Vector3 Vector3::Min(const Vector3& v1, const Vector3& v2)
{
  using namespace DirectX;
  XMVECTOR x1 = XMLoadFloat3(&v1);
  XMVECTOR x2 = XMLoadFloat3(&v2);
  XMVECTOR X = XMVectorMin(x1, x2);

  Vector3 result;
  XMStoreFloat3(&result, X);
  return result;
}

void Vector3::Max(const Vector3& v1, const Vector3& v2, Vector3& result)
{
  using namespace DirectX;
  XMVECTOR x1 = XMLoadFloat3(&v1);
  XMVECTOR x2 = XMLoadFloat3(&v2);
  XMVECTOR X = XMVectorMax(x1, x2);
  XMStoreFloat3(&result, X);
}

Vector3 Vector3::Max(const Vector3& v1, const Vector3& v2)
{
  using namespace DirectX;
  XMVECTOR x1 = XMLoadFloat3(&v1);
  XMVECTOR x2 = XMLoadFloat3(&v2);
  XMVECTOR X = XMVectorMax(x1, x2);

  Vector3 result;
  XMStoreFloat3(&result, X);
  return result;
}

void Vector3::Lerp(const Vector3& v1, const Vector3& v2, float t, Vector3& result)
{
  using namespace DirectX;
  XMVECTOR x1 = XMLoadFloat3(&v1);
  XMVECTOR x2 = XMLoadFloat3(&v2);
  XMVECTOR X = XMVectorLerp(x1, x2, t);
  XMStoreFloat3(&result, X);
}

Vector3 Vector3::Lerp(const Vector3& v1, const Vector3& v2, float t)
{
  using namespace DirectX;
  XMVECTOR x1 = XMLoadFloat3(&v1);
  XMVECTOR x2 = XMLoadFloat3(&v2);
  XMVECTOR X = XMVectorLerp(x1, x2, t);

  Vector3 result;
  XMStoreFloat3(&result, X);
  return result;
}

void Vector3::SmoothStep(const Vector3& v1, const Vector3& v2, float t, Vector3& result)
{
  using namespace DirectX;
  t = (t > 1.0f) ? 1.0f : ((t < 0.0f) ? 0.0f : t);  // Clamp value to 0 to 1
  t = t*t*(3.f - 2.f*t);
  XMVECTOR x1 = XMLoadFloat3(&v1);
  XMVECTOR x2 = XMLoadFloat3(&v2);
  XMVECTOR X = XMVectorLerp(x1, x2, t);
  XMStoreFloat3(&result, X);
}

Vector3 Vector3::SmoothStep(const Vector3& v1, const Vector3& v2, float t)
{
  using namespace DirectX;
  t = (t > 1.0f) ? 1.0f : ((t < 0.0f) ? 0.0f : t);  // Clamp value to 0 to 1
  t = t*t*(3.f - 2.f*t);
  XMVECTOR x1 = XMLoadFloat3(&v1);
  XMVECTOR x2 = XMLoadFloat3(&v2);
  XMVECTOR X = XMVectorLerp(x1, x2, t);

  Vector3 result;
  XMStoreFloat3(&result, X);
  return result;
}

void Vector3::Barycentric(const Vector3& v1, const Vector3& v2, const Vector3& v3, float f, float g, Vector3& result)
{
  using namespace DirectX;
  XMVECTOR x1 = XMLoadFloat3(&v1);
  XMVECTOR x2 = XMLoadFloat3(&v2);
  XMVECTOR x3 = XMLoadFloat3(&v3);
  XMVECTOR X = XMVectorBaryCentric(x1, x2, x3, f, g);
  XMStoreFloat3(&result, X);
}

Vector3 Vector3::Barycentric(const Vector3& v1, const Vector3& v2, const Vector3& v3, float f, float g)
{
  using namespace DirectX;
  XMVECTOR x1 = XMLoadFloat3(&v1);
  XMVECTOR x2 = XMLoadFloat3(&v2);
  XMVECTOR x3 = XMLoadFloat3(&v3);
  XMVECTOR X = XMVectorBaryCentric(x1, x2, x3, f, g);

  Vector3 result;
  XMStoreFloat3(&result, X);
  return result;
}

void Vector3::CatmullRom(const Vector3& v1, const Vector3& v2, const Vector3& v3, const Vector3& v4, float t, Vector3& result)
{
  using namespace DirectX;
  XMVECTOR x1 = XMLoadFloat3(&v1);
  XMVECTOR x2 = XMLoadFloat3(&v2);
  XMVECTOR x3 = XMLoadFloat3(&v3);
  XMVECTOR x4 = XMLoadFloat3(&v4);
  XMVECTOR X = XMVectorCatmullRom(x1, x2, x3, x4, t);
  XMStoreFloat3(&result, X);
}

Vector3 Vector3::CatmullRom(const Vector3& v1, const Vector3& v2, const Vector3& v3, const Vector3& v4, float t)
{
  using namespace DirectX;
  XMVECTOR x1 = XMLoadFloat3(&v1);
  XMVECTOR x2 = XMLoadFloat3(&v2);
  XMVECTOR x3 = XMLoadFloat3(&v3);
  XMVECTOR x4 = XMLoadFloat3(&v4);
  XMVECTOR X = XMVectorCatmullRom(x1, x2, x3, x4, t);

  Vector3 result;
  XMStoreFloat3(&result, X);
  return result;
}

void Vector3::Hermite(const Vector3& v1, const Vector3& t1, const Vector3& v2, const Vector3& t2, float t, Vector3& result)
{
  using namespace DirectX;
  XMVECTOR x1 = XMLoadFloat3(&v1);
  XMVECTOR x2 = XMLoadFloat3(&t1);
  XMVECTOR x3 = XMLoadFloat3(&v2);
  XMVECTOR x4 = XMLoadFloat3(&t2);
  XMVECTOR X = XMVectorHermite(x1, x2, x3, x4, t);
  XMStoreFloat3(&result, X);
}

Vector3 Vector3::Hermite(const Vector3& v1, const Vector3& t1, const Vector3& v2, const Vector3& t2, float t)
{
  using namespace DirectX;
  XMVECTOR x1 = XMLoadFloat3(&v1);
  XMVECTOR x2 = XMLoadFloat3(&t1);
  XMVECTOR x3 = XMLoadFloat3(&v2);
  XMVECTOR x4 = XMLoadFloat3(&t2);
  XMVECTOR X = XMVectorHermite(x1, x2, x3, x4, t);

  Vector3 result;
  XMStoreFloat3(&result, X);
  return result;
}

void Vector3::Reflect(const Vector3& ivec, const Vector3& nvec, Vector3& result)
{
  using namespace DirectX;
  XMVECTOR i = XMLoadFloat3(&ivec);
  XMVECTOR n = XMLoadFloat3(&nvec);
  XMVECTOR X = XMVector3Reflect(i, n);
  XMStoreFloat3(&result, X);
}

Vector3 Vector3::Reflect(const Vector3& ivec, const Vector3& nvec)
{
  using namespace DirectX;
  XMVECTOR i = XMLoadFloat3(&ivec);
  XMVECTOR n = XMLoadFloat3(&nvec);
  XMVECTOR X = XMVector3Reflect(i, n);

  Vector3 result;
  XMStoreFloat3(&result, X);
  return result;
}

void Vector3::Refract(const Vector3& ivec, const Vector3& nvec, float refractionIndex, Vector3& result)
{
  using namespace DirectX;
  XMVECTOR i = XMLoadFloat3(&ivec);
  XMVECTOR n = XMLoadFloat3(&nvec);
  XMVECTOR X = XMVector3Refract(i, n, refractionIndex);
  XMStoreFloat3(&result, X);
}

Vector3 Vector3::Refract(const Vector3& ivec, const Vector3& nvec, float refractionIndex)
{
  using namespace DirectX;
  XMVECTOR i = XMLoadFloat3(&ivec);
  XMVECTOR n = XMLoadFloat3(&nvec);
  XMVECTOR X = XMVector3Refract(i, n, refractionIndex);

  Vector3 result;
  XMStoreFloat3(&result, X);
  return result;
}

void Vector3::Transform(const Vector3& v, const Quaternion& quat, Vector3& result)
{
  using namespace DirectX;
  XMVECTOR v1 = XMLoadFloat3(&v);
  XMVECTOR q = XMLoadFloat4(&quat);
  XMVECTOR X = XMVector3Rotate(v1, q);
  XMStoreFloat3(&result, X);
}

Vector3 Vector3::Transform(const Vector3& v, const Quaternion& quat)
{
  using namespace DirectX;
  XMVECTOR v1 = XMLoadFloat3(&v);
  XMVECTOR q = XMLoadFloat4(&quat);
  XMVECTOR X = XMVector3Rotate(v1, q);

  Vector3 result;
  XMStoreFloat3(&result, X);
  return result;
}

void Vector3::Transform(const Vector3& v, const Matrix& m, Vector3& result)
{
  using namespace DirectX;
  XMVECTOR v1 = XMLoadFloat3(&v);
  XMMATRIX M = XMLoadFloat4x4(&m);
  XMVECTOR X = XMVector3TransformCoord(v1, M);
  XMStoreFloat3(&result, X);
}

Vector3 Vector3::Transform(const Vector3& v, const Matrix& m)
{
  using namespace DirectX;
  XMVECTOR v1 = XMLoadFloat3(&v);
  XMMATRIX M = XMLoadFloat4x4(&m);
  XMVECTOR X = XMVector3TransformCoord(v1, M);

  Vector3 result;
  XMStoreFloat3(&result, X);
  return result;
}

_Use_decl_annotations_
void Vector3::Transform(const Vector3* varray, size_t count, const Matrix& m, Vector3* resultArray)
{
  using namespace DirectX;
  XMMATRIX M = XMLoadFloat4x4(&m);
  XMVector3TransformCoordStream(resultArray, sizeof(XMFLOAT3), varray, sizeof(XMFLOAT3), count, M);
}

void Vector3::Transform(const Vector3& v, const Matrix& m, Vector4& result)
{
  using namespace DirectX;
  XMVECTOR v1 = XMLoadFloat3(&v);
  XMMATRIX M = XMLoadFloat4x4(&m);
  XMVECTOR X = XMVector3Transform(v1, M);
  XMStoreFloat4(&result, X);
}

_Use_decl_annotations_
void Vector3::Transform(const Vector3* varray, size_t count, const Matrix& m, Vector4* resultArray)
{
  using namespace DirectX;
  XMMATRIX M = XMLoadFloat4x4(&m);
  XMVector3TransformStream(resultArray, sizeof(XMFLOAT4), varray, sizeof(XMFLOAT3), count, M);
}

void Vector3::TransformNormal(const Vector3& v, const Matrix& m, Vector3& result)
{
  using namespace DirectX;
  XMVECTOR v1 = XMLoadFloat3(&v);
  XMMATRIX M = XMLoadFloat4x4(&m);
  XMVECTOR X = XMVector3TransformNormal(v1, M);
  XMStoreFloat3(&result, X);
}

Vector3 Vector3::TransformNormal(const Vector3& v, const Matrix& m)
{
  using namespace DirectX;
  XMVECTOR v1 = XMLoadFloat3(&v);
  XMMATRIX M = XMLoadFloat4x4(&m);
  XMVECTOR X = XMVector3TransformNormal(v1, M);

  Vector3 result;
  XMStoreFloat3(&result, X);
  return result;
}

_Use_decl_annotations_
void Vector3::TransformNormal(const Vector3* varray, size_t count, const Matrix& m, Vector3* resultArray)
{
  using namespace DirectX;
  XMMATRIX M = XMLoadFloat4x4(&m);
  XMVector3TransformNormalStream(resultArray, sizeof(XMFLOAT3), varray, sizeof(XMFLOAT3), count, M);
}


/****************************************************************************
*
* Vector4
*
****************************************************************************/

//------------------------------------------------------------------------------
// Comparision operators
//------------------------------------------------------------------------------

bool Vector4::operator == (const Vector4& V) const
{
  using namespace DirectX;
  XMVECTOR v1 = XMLoadFloat4(this);
  XMVECTOR v2 = XMLoadFloat4(&V);
  return XMVector4Equal(v1, v2);
}

bool Vector4::operator != (const Vector4& V) const
{
  using namespace DirectX;
  XMVECTOR v1 = XMLoadFloat4(this);
  XMVECTOR v2 = XMLoadFloat4(&V);
  return XMVector4NotEqual(v1, v2);
}

//------------------------------------------------------------------------------
// Assignment operators
//------------------------------------------------------------------------------

Vector4& Vector4::operator+= (const Vector4& V)
{
  using namespace DirectX;
  XMVECTOR v1 = XMLoadFloat4(this);
  XMVECTOR v2 = XMLoadFloat4(&V);
  XMVECTOR X = XMVectorAdd(v1, v2);
  XMStoreFloat4(this, X);
  return *this;
}

Vector4& Vector4::operator-= (const Vector4& V)
{
  using namespace DirectX;
  XMVECTOR v1 = XMLoadFloat4(this);
  XMVECTOR v2 = XMLoadFloat4(&V);
  XMVECTOR X = XMVectorSubtract(v1, v2);
  XMStoreFloat4(this, X);
  return *this;
}

Vector4& Vector4::operator*= (const Vector4& V)
{
  using namespace DirectX;
  XMVECTOR v1 = XMLoadFloat4(this);
  XMVECTOR v2 = XMLoadFloat4(&V);
  XMVECTOR X = XMVectorMultiply(v1, v2);
  XMStoreFloat4(this, X);
  return *this;
}

Vector4& Vector4::operator*= (float S)
{
  using namespace DirectX;
  XMVECTOR v1 = XMLoadFloat4(this);
  XMVECTOR X = XMVectorScale(v1, S);
  XMStoreFloat4(this, X);
  return *this;
}

Vector4& Vector4::operator/= (float S)
{
  using namespace DirectX;
  assert(S != 0.0f);
  XMVECTOR v1 = XMLoadFloat4(this);
  XMVECTOR X = XMVectorScale(v1, 1.f / S);
  XMStoreFloat4(this, X);
  return *this;
}

//------------------------------------------------------------------------------
// Urnary operators
//------------------------------------------------------------------------------

Vector4 Vector4::operator- () const
{
  using namespace DirectX;
  XMVECTOR v1 = XMLoadFloat4(this);
  XMVECTOR X = XMVectorNegate(v1);
  Vector4 R;
  XMStoreFloat4(&R, X);
  return R;
}

//------------------------------------------------------------------------------
// Binary operators
//------------------------------------------------------------------------------

Vector4 operator+ (const Vector4& V1, const Vector4& V2)
{
  using namespace DirectX;
  XMVECTOR v1 = XMLoadFloat4(&V1);
  XMVECTOR v2 = XMLoadFloat4(&V2);
  XMVECTOR X = XMVectorAdd(v1, v2);
  Vector4 R;
  XMStoreFloat4(&R, X);
  return R;
}

Vector4 operator- (const Vector4& V1, const Vector4& V2)
{
  using namespace DirectX;
  XMVECTOR v1 = XMLoadFloat4(&V1);
  XMVECTOR v2 = XMLoadFloat4(&V2);
  XMVECTOR X = XMVectorSubtract(v1, v2);
  Vector4 R;
  XMStoreFloat4(&R, X);
  return R;
}

Vector4 operator* (const Vector4& V1, const Vector4& V2)
{
  using namespace DirectX;
  XMVECTOR v1 = XMLoadFloat4(&V1);
  XMVECTOR v2 = XMLoadFloat4(&V2);
  XMVECTOR X = XMVectorMultiply(v1, v2);
  Vector4 R;
  XMStoreFloat4(&R, X);
  return R;
}

Vector4 operator* (const Vector4& V, float S)
{
  using namespace DirectX;
  XMVECTOR v1 = XMLoadFloat4(&V);
  XMVECTOR X = XMVectorScale(v1, S);
  Vector4 R;
  XMStoreFloat4(&R, X);
  return R;
}

Vector4 operator/ (const Vector4& V1, const Vector4& V2)
{
  using namespace DirectX;
  XMVECTOR v1 = XMLoadFloat4(&V1);
  XMVECTOR v2 = XMLoadFloat4(&V2);
  XMVECTOR X = XMVectorDivide(v1, v2);
  Vector4 R;
  XMStoreFloat4(&R, X);
  return R;
}

Vector4 operator* (float S, const Vector4& V)
{
  using namespace DirectX;
  XMVECTOR v1 = XMLoadFloat4(&V);
  XMVECTOR X = XMVectorScale(v1, S);
  Vector4 R;
  XMStoreFloat4(&R, X);
  return R;
}

//------------------------------------------------------------------------------
// Vector operations
//------------------------------------------------------------------------------

bool Vector4::InBounds(const Vector4& Bounds) const
{
  using namespace DirectX;
  XMVECTOR v1 = XMLoadFloat4(this);
  XMVECTOR v2 = XMLoadFloat4(&Bounds);
  return XMVector4InBounds(v1, v2);
}

float Vector4::Length() const
{
  using namespace DirectX;
  XMVECTOR v1 = XMLoadFloat4(this);
  XMVECTOR X = XMVector4Length(v1);
  return XMVectorGetX(X);
}

float Vector4::LengthSquared() const
{
  using namespace DirectX;
  XMVECTOR v1 = XMLoadFloat4(this);
  XMVECTOR X = XMVector4LengthSq(v1);
  return XMVectorGetX(X);
}

float Vector4::Dot(const Vector4& V) const
{
  using namespace DirectX;
  XMVECTOR v1 = XMLoadFloat4(this);
  XMVECTOR v2 = XMLoadFloat4(&V);
  XMVECTOR X = XMVector4Dot(v1, v2);
  return XMVectorGetX(X);
}

void Vector4::Cross(const Vector4& v1, const Vector4& v2, Vector4& result) const
{
  using namespace DirectX;
  XMVECTOR x1 = XMLoadFloat4(this);
  XMVECTOR x2 = XMLoadFloat4(&v1);
  XMVECTOR x3 = XMLoadFloat4(&v2);
  XMVECTOR R = XMVector4Cross(x1, x2, x3);
  XMStoreFloat4(&result, R);
}

Vector4 Vector4::Cross(const Vector4& v1, const Vector4& v2) const
{
  using namespace DirectX;
  XMVECTOR x1 = XMLoadFloat4(this);
  XMVECTOR x2 = XMLoadFloat4(&v1);
  XMVECTOR x3 = XMLoadFloat4(&v2);
  XMVECTOR R = XMVector4Cross(x1, x2, x3);

  Vector4 result;
  XMStoreFloat4(&result, R);
  return result;
}

void Vector4::Normalize()
{
  using namespace DirectX;
  XMVECTOR v1 = XMLoadFloat4(this);
  XMVECTOR X = XMVector4Normalize(v1);
  XMStoreFloat4(this, X);
}

void Vector4::Normalize(Vector4& result) const
{
  using namespace DirectX;
  XMVECTOR v1 = XMLoadFloat4(this);
  XMVECTOR X = XMVector4Normalize(v1);
  XMStoreFloat4(&result, X);
}

void Vector4::Clamp(const Vector4& vmin, const Vector4& vmax)
{
  using namespace DirectX;
  XMVECTOR v1 = XMLoadFloat4(this);
  XMVECTOR v2 = XMLoadFloat4(&vmin);
  XMVECTOR v3 = XMLoadFloat4(&vmax);
  XMVECTOR X = XMVectorClamp(v1, v2, v3);
  XMStoreFloat4(this, X);
}

void Vector4::Clamp(const Vector4& vmin, const Vector4& vmax, Vector4& result) const
{
  using namespace DirectX;
  XMVECTOR v1 = XMLoadFloat4(this);
  XMVECTOR v2 = XMLoadFloat4(&vmin);
  XMVECTOR v3 = XMLoadFloat4(&vmax);
  XMVECTOR X = XMVectorClamp(v1, v2, v3);
  XMStoreFloat4(&result, X);
}

//------------------------------------------------------------------------------
// Static functions
//------------------------------------------------------------------------------

float Vector4::Distance(const Vector4& v1, const Vector4& v2)
{
  using namespace DirectX;
  XMVECTOR x1 = XMLoadFloat4(&v1);
  XMVECTOR x2 = XMLoadFloat4(&v2);
  XMVECTOR V = XMVectorSubtract(x2, x1);
  XMVECTOR X = XMVector4Length(V);
  return XMVectorGetX(X);
}

float Vector4::DistanceSquared(const Vector4& v1, const Vector4& v2)
{
  using namespace DirectX;
  XMVECTOR x1 = XMLoadFloat4(&v1);
  XMVECTOR x2 = XMLoadFloat4(&v2);
  XMVECTOR V = XMVectorSubtract(x2, x1);
  XMVECTOR X = XMVector4LengthSq(V);
  return XMVectorGetX(X);
}

void Vector4::Min(const Vector4& v1, const Vector4& v2, Vector4& result)
{
  using namespace DirectX;
  XMVECTOR x1 = XMLoadFloat4(&v1);
  XMVECTOR x2 = XMLoadFloat4(&v2);
  XMVECTOR X = XMVectorMin(x1, x2);
  XMStoreFloat4(&result, X);
}

Vector4 Vector4::Min(const Vector4& v1, const Vector4& v2)
{
  using namespace DirectX;
  XMVECTOR x1 = XMLoadFloat4(&v1);
  XMVECTOR x2 = XMLoadFloat4(&v2);
  XMVECTOR X = XMVectorMin(x1, x2);

  Vector4 result;
  XMStoreFloat4(&result, X);
  return result;
}

void Vector4::Max(const Vector4& v1, const Vector4& v2, Vector4& result)
{
  using namespace DirectX;
  XMVECTOR x1 = XMLoadFloat4(&v1);
  XMVECTOR x2 = XMLoadFloat4(&v2);
  XMVECTOR X = XMVectorMax(x1, x2);
  XMStoreFloat4(&result, X);
}

Vector4 Vector4::Max(const Vector4& v1, const Vector4& v2)
{
  using namespace DirectX;
  XMVECTOR x1 = XMLoadFloat4(&v1);
  XMVECTOR x2 = XMLoadFloat4(&v2);
  XMVECTOR X = XMVectorMax(x1, x2);

  Vector4 result;
  XMStoreFloat4(&result, X);
  return result;
}

void Vector4::Lerp(const Vector4& v1, const Vector4& v2, float t, Vector4& result)
{
  using namespace DirectX;
  XMVECTOR x1 = XMLoadFloat4(&v1);
  XMVECTOR x2 = XMLoadFloat4(&v2);
  XMVECTOR X = XMVectorLerp(x1, x2, t);
  XMStoreFloat4(&result, X);
}

Vector4 Vector4::Lerp(const Vector4& v1, const Vector4& v2, float t)
{
  using namespace DirectX;
  XMVECTOR x1 = XMLoadFloat4(&v1);
  XMVECTOR x2 = XMLoadFloat4(&v2);
  XMVECTOR X = XMVectorLerp(x1, x2, t);

  Vector4 result;
  XMStoreFloat4(&result, X);
  return result;
}

void Vector4::SmoothStep(const Vector4& v1, const Vector4& v2, float t, Vector4& result)
{
  using namespace DirectX;
  t = (t > 1.0f) ? 1.0f : ((t < 0.0f) ? 0.0f : t);  // Clamp value to 0 to 1
  t = t*t*(3.f - 2.f*t);
  XMVECTOR x1 = XMLoadFloat4(&v1);
  XMVECTOR x2 = XMLoadFloat4(&v2);
  XMVECTOR X = XMVectorLerp(x1, x2, t);
  XMStoreFloat4(&result, X);
}

Vector4 Vector4::SmoothStep(const Vector4& v1, const Vector4& v2, float t)
{
  using namespace DirectX;
  t = (t > 1.0f) ? 1.0f : ((t < 0.0f) ? 0.0f : t);  // Clamp value to 0 to 1
  t = t*t*(3.f - 2.f*t);
  XMVECTOR x1 = XMLoadFloat4(&v1);
  XMVECTOR x2 = XMLoadFloat4(&v2);
  XMVECTOR X = XMVectorLerp(x1, x2, t);

  Vector4 result;
  XMStoreFloat4(&result, X);
  return result;
}

void Vector4::Barycentric(const Vector4& v1, const Vector4& v2, const Vector4& v3, float f, float g, Vector4& result)
{
  using namespace DirectX;
  XMVECTOR x1 = XMLoadFloat4(&v1);
  XMVECTOR x2 = XMLoadFloat4(&v2);
  XMVECTOR x3 = XMLoadFloat4(&v3);
  XMVECTOR X = XMVectorBaryCentric(x1, x2, x3, f, g);
  XMStoreFloat4(&result, X);
}

Vector4 Vector4::Barycentric(const Vector4& v1, const Vector4& v2, const Vector4& v3, float f, float g)
{
  using namespace DirectX;
  XMVECTOR x1 = XMLoadFloat4(&v1);
  XMVECTOR x2 = XMLoadFloat4(&v2);
  XMVECTOR x3 = XMLoadFloat4(&v3);
  XMVECTOR X = XMVectorBaryCentric(x1, x2, x3, f, g);

  Vector4 result;
  XMStoreFloat4(&result, X);
  return result;
}

void Vector4::CatmullRom(const Vector4& v1, const Vector4& v2, const Vector4& v3, const Vector4& v4, float t, Vector4& result)
{
  using namespace DirectX;
  XMVECTOR x1 = XMLoadFloat4(&v1);
  XMVECTOR x2 = XMLoadFloat4(&v2);
  XMVECTOR x3 = XMLoadFloat4(&v3);
  XMVECTOR x4 = XMLoadFloat4(&v4);
  XMVECTOR X = XMVectorCatmullRom(x1, x2, x3, x4, t);
  XMStoreFloat4(&result, X);
}

Vector4 Vector4::CatmullRom(const Vector4& v1, const Vector4& v2, const Vector4& v3, const Vector4& v4, float t)
{
  using namespace DirectX;
  XMVECTOR x1 = XMLoadFloat4(&v1);
  XMVECTOR x2 = XMLoadFloat4(&v2);
  XMVECTOR x3 = XMLoadFloat4(&v3);
  XMVECTOR x4 = XMLoadFloat4(&v4);
  XMVECTOR X = XMVectorCatmullRom(x1, x2, x3, x4, t);

  Vector4 result;
  XMStoreFloat4(&result, X);
  return result;
}

void Vector4::Hermite(const Vector4& v1, const Vector4& t1, const Vector4& v2, const Vector4& t2, float t, Vector4& result)
{
  using namespace DirectX;
  XMVECTOR x1 = XMLoadFloat4(&v1);
  XMVECTOR x2 = XMLoadFloat4(&t1);
  XMVECTOR x3 = XMLoadFloat4(&v2);
  XMVECTOR x4 = XMLoadFloat4(&t2);
  XMVECTOR X = XMVectorHermite(x1, x2, x3, x4, t);
  XMStoreFloat4(&result, X);
}

Vector4 Vector4::Hermite(const Vector4& v1, const Vector4& t1, const Vector4& v2, const Vector4& t2, float t)
{
  using namespace DirectX;
  XMVECTOR x1 = XMLoadFloat4(&v1);
  XMVECTOR x2 = XMLoadFloat4(&t1);
  XMVECTOR x3 = XMLoadFloat4(&v2);
  XMVECTOR x4 = XMLoadFloat4(&t2);
  XMVECTOR X = XMVectorHermite(x1, x2, x3, x4, t);

  Vector4 result;
  XMStoreFloat4(&result, X);
  return result;
}

void Vector4::Reflect(const Vector4& ivec, const Vector4& nvec, Vector4& result)
{
  using namespace DirectX;
  XMVECTOR i = XMLoadFloat4(&ivec);
  XMVECTOR n = XMLoadFloat4(&nvec);
  XMVECTOR X = XMVector4Reflect(i, n);
  XMStoreFloat4(&result, X);
}

Vector4 Vector4::Reflect(const Vector4& ivec, const Vector4& nvec)
{
  using namespace DirectX;
  XMVECTOR i = XMLoadFloat4(&ivec);
  XMVECTOR n = XMLoadFloat4(&nvec);
  XMVECTOR X = XMVector4Reflect(i, n);

  Vector4 result;
  XMStoreFloat4(&result, X);
  return result;
}

void Vector4::Refract(const Vector4& ivec, const Vector4& nvec, float refractionIndex, Vector4& result)
{
  using namespace DirectX;
  XMVECTOR i = XMLoadFloat4(&ivec);
  XMVECTOR n = XMLoadFloat4(&nvec);
  XMVECTOR X = XMVector4Refract(i, n, refractionIndex);
  XMStoreFloat4(&result, X);
}

Vector4 Vector4::Refract(const Vector4& ivec, const Vector4& nvec, float refractionIndex)
{
  using namespace DirectX;
  XMVECTOR i = XMLoadFloat4(&ivec);
  XMVECTOR n = XMLoadFloat4(&nvec);
  XMVECTOR X = XMVector4Refract(i, n, refractionIndex);

  Vector4 result;
  XMStoreFloat4(&result, X);
  return result;
}

void Vector4::Transform(const Vector2& v, const Quaternion& quat, Vector4& result)
{
  using namespace DirectX;
  XMVECTOR v1 = XMLoadFloat2(&v);
  XMVECTOR q = XMLoadFloat4(&quat);
  XMVECTOR X = XMVector3Rotate(v1, q);
  X = XMVectorSelect(g_XMIdentityR3, X, g_XMSelect1110); // result.w = 1.f
  XMStoreFloat4(&result, X);
}

Vector4 Vector4::Transform(const Vector2& v, const Quaternion& quat)
{
  using namespace DirectX;
  XMVECTOR v1 = XMLoadFloat2(&v);
  XMVECTOR q = XMLoadFloat4(&quat);
  XMVECTOR X = XMVector3Rotate(v1, q);
  X = XMVectorSelect(g_XMIdentityR3, X, g_XMSelect1110); // result.w = 1.f

  Vector4 result;
  XMStoreFloat4(&result, X);
  return result;
}

void Vector4::Transform(const Vector3& v, const Quaternion& quat, Vector4& result)
{
  using namespace DirectX;
  XMVECTOR v1 = XMLoadFloat3(&v);
  XMVECTOR q = XMLoadFloat4(&quat);
  XMVECTOR X = XMVector3Rotate(v1, q);
  X = XMVectorSelect(g_XMIdentityR3, X, g_XMSelect1110); // result.w = 1.f
  XMStoreFloat4(&result, X);
}

Vector4 Vector4::Transform(const Vector3& v, const Quaternion& quat)
{
  using namespace DirectX;
  XMVECTOR v1 = XMLoadFloat3(&v);
  XMVECTOR q = XMLoadFloat4(&quat);
  XMVECTOR X = XMVector3Rotate(v1, q);
  X = XMVectorSelect(g_XMIdentityR3, X, g_XMSelect1110); // result.w = 1.f

  Vector4 result;
  XMStoreFloat4(&result, X);
  return result;
}

void Vector4::Transform(const Vector4& v, const Quaternion& quat, Vector4& result)
{
  using namespace DirectX;
  XMVECTOR v1 = XMLoadFloat4(&v);
  XMVECTOR q = XMLoadFloat4(&quat);
  XMVECTOR X = XMVector3Rotate(v1, q);
  X = XMVectorSelect(v1, X, g_XMSelect1110); // result.w = v.w
  XMStoreFloat4(&result, X);
}

Vector4 Vector4::Transform(const Vector4& v, const Quaternion& quat)
{
  using namespace DirectX;
  XMVECTOR v1 = XMLoadFloat4(&v);
  XMVECTOR q = XMLoadFloat4(&quat);
  XMVECTOR X = XMVector3Rotate(v1, q);
  X = XMVectorSelect(v1, X, g_XMSelect1110); // result.w = v.w

  Vector4 result;
  XMStoreFloat4(&result, X);
  return result;
}

void Vector4::Transform(const Vector4& v, const Matrix& m, Vector4& result)
{
  using namespace DirectX;
  XMVECTOR v1 = XMLoadFloat4(&v);
  XMMATRIX M = XMLoadFloat4x4(&m);
  XMVECTOR X = XMVector4Transform(v1, M);
  XMStoreFloat4(&result, X);
}

Vector4 Vector4::Transform(const Vector4& v, const Matrix& m)
{
  using namespace DirectX;
  XMVECTOR v1 = XMLoadFloat4(&v);
  XMMATRIX M = XMLoadFloat4x4(&m);
  XMVECTOR X = XMVector4Transform(v1, M);

  Vector4 result;
  XMStoreFloat4(&result, X);
  return result;
}

_Use_decl_annotations_
void Vector4::Transform(const Vector4* varray, size_t count, const Matrix& m, Vector4* resultArray)
{
  using namespace DirectX;
  XMMATRIX M = XMLoadFloat4x4(&m);
  XMVector4TransformStream(resultArray, sizeof(XMFLOAT4), varray, sizeof(XMFLOAT4), count, M);
}

}  // namespace LL3D
}  // namespace Math

