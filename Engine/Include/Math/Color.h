#pragma once

#include <functional>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include "Math/Vector.h"

namespace LL3D {
namespace Math {

using namespace DirectX;

//------------------------------------------------------------------------------
// Color
struct Color : public XMFLOAT4
{
  Color() : XMFLOAT4(0, 0, 0, 1.f) {}
  Color(float _r, float _g, float _b) : XMFLOAT4(_r, _g, _b, 1.f) {}
  Color(float _r, float _g, float _b, float _a) : XMFLOAT4(_r, _g, _b, _a) {}
  explicit Color(const Vector3& clr) : XMFLOAT4(clr.x, clr.y, clr.z, 1.f) {}
  explicit Color(const Vector4& clr) : XMFLOAT4(clr.x, clr.y, clr.z, clr.w) {}
  explicit Color(_In_reads_(4) const float *pArray) : XMFLOAT4(pArray) {}
  Color(FXMVECTOR V) { XMStoreFloat4(this, V); }
  Color(const XMFLOAT4& c) { this->x = c.x; this->y = c.y; this->z = c.z; this->w = c.w; }

  explicit Color(const DirectX::PackedVector::XMCOLOR& Packed);
  // BGRA Direct3D 9 D3DCOLOR packed color

  explicit Color(const DirectX::PackedVector::XMUBYTEN4& Packed);
  // RGBA XNA Game Studio packed color

  operator XMVECTOR() const { return XMLoadFloat4(this); }
  operator const float*() const { return reinterpret_cast<const float*>(this); }

  // Comparison operators
  bool operator == (const Color& c) const;
  bool operator != (const Color& c) const;

  // Assignment operators
  Color& operator= (const Color& c) { x = c.x; y = c.y; z = c.z; w = c.w; return *this; }
  Color& operator= (const XMFLOAT4& c) { x = c.x; y = c.y; z = c.z; w = c.w; return *this; }
  Color& operator= (const DirectX::PackedVector::XMCOLOR& Packed);
  Color& operator= (const DirectX::PackedVector::XMUBYTEN4& Packed);
  Color& operator+= (const Color& c);
  Color& operator-= (const Color& c);
  Color& operator*= (const Color& c);
  Color& operator*= (float S);
  Color& operator/= (const Color& c);

  // Unary operators
  Color operator+ () const { return *this; }
  Color operator- () const;

  // Properties
  float R() const { return x; }
  void R(float r) { x = r; }

  float G() const { return y; }
  void G(float g) { y = g; }

  float B() const { return z; }
  void B(float b) { z = b; }

  float A() const { return w; }
  void A(float a) { w = a; }

  // Color operations
  DirectX::PackedVector::XMCOLOR BGRA() const;
  DirectX::PackedVector::XMUBYTEN4 RGBA() const;

  Vector3 ToVector3() const;
  Vector4 ToVector4() const;

  void Negate();
  void Negate(Color& result) const;

  void Saturate();
  void Saturate(Color& result) const;

  void Premultiply();
  void Premultiply(Color& result) const;

  void AdjustSaturation(float sat);
  void AdjustSaturation(float sat, Color& result) const;

  void AdjustContrast(float contrast);
  void AdjustContrast(float contrast, Color& result) const;

  // Static functions
  static void Modulate(const Color& c1, const Color& c2, Color& result);
  static Color Modulate(const Color& c1, const Color& c2);

  static void Lerp(const Color& c1, const Color& c2, float t, Color& result);
  static Color Lerp(const Color& c1, const Color& c2, float t);

  // Constants
  static const Color White;
  static const Color Black;
  static const Color Red;
  static const Color Green;
  static const Color Blue;
  static const Color Yellow;
  static const Color Cyan;
  static const Color Magenta;
  static const Color Silver;
  static const Color LightSteelBlue;
  static const Color Grey;
};

// Binary operators
extern Color operator+ (const Color& C1, const Color& C2);
extern Color operator- (const Color& C1, const Color& C2);
extern Color operator* (const Color& C1, const Color& C2);
extern Color operator* (const Color& C, float S);
extern Color operator/ (const Color& C1, const Color& C2);
extern Color operator* (float S, const Color& C);

}  // namespace Math
}  // namespace LL3D

//------------------------------------------------------------------------------
// Support for Math and Standard C++ Library containers
namespace std {

template<> struct less<LL3D::Math::Color>
{
  bool operator()(const LL3D::Math::Color& C1, const LL3D::Math::Color& C2) const
  {
    return ((C1.x < C2.x)
      || ((C1.x == C2.x) && (C1.y < C2.y))
      || ((C1.x == C2.x) && (C1.y == C2.y) && (C1.z < C2.z))
      || ((C1.x == C2.x) && (C1.y == C2.y) && (C1.z == C2.z) && (C1.w < C2.w)));
  }
};

}  // namespace std