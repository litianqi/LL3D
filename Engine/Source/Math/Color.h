#pragma once

namespace LL3D {
namespace Math {

//------------------------------------------------------------------------------
// Color3 : Color with Alpha = 1.f
struct Color3 : public DirectX::XMFLOAT4
{
  Color3() : XMFLOAT4(0, 0, 0, 1.f) {}
  Color3(float _r, float _g, float _b) : XMFLOAT4(_r, _g, _b, 1.f) {}
  explicit Color3(const Vector3& clr) : XMFLOAT4(clr.x, clr.y, clr.z, 1.f) {}

  //>
  // Conversion operators
  //
  operator DirectX::XMVECTOR() const { return XMLoadFloat4(this); }
  operator const float*() const { return reinterpret_cast<const float*>(this); }

  //>
  // Comparison operators
  //
  bool operator == (const Color3& c) const;
  bool operator != (const Color3& c) const;

  //>
  // Assignment operators
  //
  Color3& operator= (const Color3& c) { x = c.x; y = c.y; z = c.z; w = c.w; return *this; }
  Color3& operator= (const XMFLOAT4& c) { x = c.x; y = c.y; z = c.z; w = c.w; return *this; }
  Color3& operator= (const DirectX::PackedVector::XMCOLOR& Packed);
  Color3& operator= (const DirectX::PackedVector::XMUBYTEN4& Packed);
  Color3& operator+= (const Color3& c);
  Color3& operator-= (const Color3& c);
  Color3& operator*= (const Color3& c);
  Color3& operator*= (float S);
  Color3& operator/= (const Color3& c);

  //>
  // Unary operators
  //
  Color3 operator+ () const { return *this; }
  Color3 operator- () const;

  //--------------------------------------
  // Properties

  float R() const { return x; }
  void R(float r) { x = r; }

  float G() const { return y; }
  void G(float g) { y = g; }

  float B() const { return z; }
  void B(float b) { z = b; }

  Vector3 ToVector3() const;
  Vector4 ToVector4() const;

  void Negate();
  void Negate(Color3& result) const;

  void Saturate();
  void Saturate(Color3& result) const;

  void Premultiply();
  void Premultiply(Color3& result) const;

  void AdjustSaturation(float sat);
  void AdjustSaturation(float sat, Color3& result) const;

  void AdjustContrast(float contrast);
  void AdjustContrast(float contrast, Color3& result) const;

  //--------------------------------------
  // Static functions

  static void Modulate(const Color3& c1, const Color3& c2, Color3& result);
  static Color3 Modulate(const Color3& c1, const Color3& c2);

  static void Lerp(const Color3& c1, const Color3& c2, float t, Color3& result);
  static Color3 Lerp(const Color3& c1, const Color3& c2, float t);
};

}  // namespace Math
}  // namespace LL3D