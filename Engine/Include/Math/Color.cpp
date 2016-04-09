#include "Math/Color.h"

namespace LL3D {
namespace Math {

/****************************************************************************
*
* Color
*
****************************************************************************/

Color::Color(const DirectX::PackedVector::XMCOLOR& Packed)
{
  using namespace DirectX;
  XMStoreFloat4(this, PackedVector::XMLoadColor(&Packed));
}

Color::Color(const DirectX::PackedVector::XMUBYTEN4& Packed)
{
  using namespace DirectX;
  XMStoreFloat4(this, PackedVector::XMLoadUByteN4(&Packed));
}

//------------------------------------------------------------------------------
// Comparision operators
//------------------------------------------------------------------------------
bool Color::operator == (const Color& c) const
{
  using namespace DirectX;
  XMVECTOR c1 = XMLoadFloat4(this);
  XMVECTOR c2 = XMLoadFloat4(&c);
  return XMColorEqual(c1, c2);
}

bool Color::operator != (const Color& c) const
{
  using namespace DirectX;
  XMVECTOR c1 = XMLoadFloat4(this);
  XMVECTOR c2 = XMLoadFloat4(&c);
  return XMColorNotEqual(c1, c2);
}

//------------------------------------------------------------------------------
// Assignment operators
//------------------------------------------------------------------------------

Color& Color::operator= (const DirectX::PackedVector::XMCOLOR& Packed)
{
  using namespace DirectX;
  XMStoreFloat4(this, PackedVector::XMLoadColor(&Packed));
  return *this;
}

Color& Color::operator= (const DirectX::PackedVector::XMUBYTEN4& Packed)
{
  using namespace DirectX;
  XMStoreFloat4(this, PackedVector::XMLoadUByteN4(&Packed));
  return *this;
}

Color& Color::operator+= (const Color& c)
{
  using namespace DirectX;
  XMVECTOR c1 = XMLoadFloat4(this);
  XMVECTOR c2 = XMLoadFloat4(&c);
  XMStoreFloat4(this, XMVectorAdd(c1, c2));
  return *this;
}

Color& Color::operator-= (const Color& c)
{
  using namespace DirectX;
  XMVECTOR c1 = XMLoadFloat4(this);
  XMVECTOR c2 = XMLoadFloat4(&c);
  XMStoreFloat4(this, XMVectorSubtract(c1, c2));
  return *this;
}

Color& Color::operator*= (const Color& c)
{
  using namespace DirectX;
  XMVECTOR c1 = XMLoadFloat4(this);
  XMVECTOR c2 = XMLoadFloat4(&c);
  XMStoreFloat4(this, XMVectorMultiply(c1, c2));
  return *this;
}

Color& Color::operator*= (float S)
{
  using namespace DirectX;
  XMVECTOR c = XMLoadFloat4(this);
  XMStoreFloat4(this, XMVectorScale(c, S));
  return *this;
}

Color& Color::operator/= (const Color& c)
{
  using namespace DirectX;
  XMVECTOR c1 = XMLoadFloat4(this);
  XMVECTOR c2 = XMLoadFloat4(&c);
  XMStoreFloat4(this, XMVectorDivide(c1, c2));
  return *this;
}

//------------------------------------------------------------------------------
// Urnary operators
//------------------------------------------------------------------------------

Color Color::operator- () const
{
  using namespace DirectX;
  XMVECTOR c = XMLoadFloat4(this);
  Color R;
  XMStoreFloat4(&R, XMVectorNegate(c));
  return R;
}

//------------------------------------------------------------------------------
// Binary operators
//------------------------------------------------------------------------------

Color operator+ (const Color& C1, const Color& C2)
{
  using namespace DirectX;
  XMVECTOR c1 = XMLoadFloat4(&C1);
  XMVECTOR c2 = XMLoadFloat4(&C2);
  Color R;
  XMStoreFloat4(&R, XMVectorAdd(c1, c2));
  return R;
}

Color operator- (const Color& C1, const Color& C2)
{
  using namespace DirectX;
  XMVECTOR c1 = XMLoadFloat4(&C1);
  XMVECTOR c2 = XMLoadFloat4(&C2);
  Color R;
  XMStoreFloat4(&R, XMVectorSubtract(c1, c2));
  return R;
}

Color operator* (const Color& C1, const Color& C2)
{
  using namespace DirectX;
  XMVECTOR c1 = XMLoadFloat4(&C1);
  XMVECTOR c2 = XMLoadFloat4(&C2);
  Color R;
  XMStoreFloat4(&R, XMVectorMultiply(c1, c2));
  return R;
}

Color operator* (const Color& C, float S)
{
  using namespace DirectX;
  XMVECTOR c = XMLoadFloat4(&C);
  Color R;
  XMStoreFloat4(&R, XMVectorScale(c, S));
  return R;
}

Color operator/ (const Color& C1, const Color& C2)
{
  using namespace DirectX;
  XMVECTOR c1 = XMLoadFloat4(&C1);
  XMVECTOR c2 = XMLoadFloat4(&C2);
  Color R;
  XMStoreFloat4(&R, XMVectorDivide(c1, c2));
  return R;
}

Color operator* (float S, const Color& C)
{
  using namespace DirectX;
  XMVECTOR c1 = XMLoadFloat4(&C);
  Color R;
  XMStoreFloat4(&R, XMVectorScale(c1, S));
  return R;
}

//------------------------------------------------------------------------------
// Color operations
//------------------------------------------------------------------------------

DirectX::PackedVector::XMCOLOR Color::BGRA() const
{
  using namespace DirectX;
  XMVECTOR clr = XMLoadFloat4(this);
  PackedVector::XMCOLOR Packed;
  PackedVector::XMStoreColor(&Packed, clr);
  return Packed;
}

DirectX::PackedVector::XMUBYTEN4 Color::RGBA() const
{
  using namespace DirectX;
  XMVECTOR clr = XMLoadFloat4(this);
  PackedVector::XMUBYTEN4 Packed;
  PackedVector::XMStoreUByteN4(&Packed, clr);
  return Packed;
}

Vector3 Color::ToVector3() const
{
  return Vector3(x, y, z);
}

Vector4 Color::ToVector4() const
{
  return Vector4(x, y, z, w);
}

void Color::Negate()
{
  using namespace DirectX;
  XMVECTOR c = XMLoadFloat4(this);
  XMStoreFloat4(this, XMColorNegative(c));
}

void Color::Negate(Color& result) const
{
  using namespace DirectX;
  XMVECTOR c = XMLoadFloat4(this);
  XMStoreFloat4(&result, XMColorNegative(c));
}

void Color::Saturate()
{
  using namespace DirectX;
  XMVECTOR c = XMLoadFloat4(this);
  XMStoreFloat4(this, XMVectorSaturate(c));
}

void Color::Saturate(Color& result) const
{
  using namespace DirectX;
  XMVECTOR c = XMLoadFloat4(this);
  XMStoreFloat4(&result, XMVectorSaturate(c));
}

void Color::Premultiply()
{
  using namespace DirectX;
  XMVECTOR c = XMLoadFloat4(this);
  XMVECTOR a = XMVectorSplatW(c);
  a = XMVectorSelect(g_XMIdentityR3, a, g_XMSelect1110);
  XMStoreFloat4(this, XMVectorMultiply(c, a));
}

void Color::Premultiply(Color& result) const
{
  using namespace DirectX;
  XMVECTOR c = XMLoadFloat4(this);
  XMVECTOR a = XMVectorSplatW(c);
  a = XMVectorSelect(g_XMIdentityR3, a, g_XMSelect1110);
  XMStoreFloat4(&result, XMVectorMultiply(c, a));
}

void Color::AdjustSaturation(float sat)
{
  using namespace DirectX;
  XMVECTOR c = XMLoadFloat4(this);
  XMStoreFloat4(this, XMColorAdjustSaturation(c, sat));
}

void Color::AdjustSaturation(float sat, Color& result) const
{
  using namespace DirectX;
  XMVECTOR c = XMLoadFloat4(this);
  XMStoreFloat4(&result, XMColorAdjustSaturation(c, sat));
}

void Color::AdjustContrast(float contrast)
{
  using namespace DirectX;
  XMVECTOR c = XMLoadFloat4(this);
  XMStoreFloat4(this, XMColorAdjustContrast(c, contrast));
}

void Color::AdjustContrast(float contrast, Color& result) const
{
  using namespace DirectX;
  XMVECTOR c = XMLoadFloat4(this);
  XMStoreFloat4(&result, XMColorAdjustContrast(c, contrast));
}

//------------------------------------------------------------------------------
// Static functions
//------------------------------------------------------------------------------

void Color::Modulate(const Color& c1, const Color& c2, Color& result)
{
  using namespace DirectX;
  XMVECTOR C0 = XMLoadFloat4(&c1);
  XMVECTOR C1 = XMLoadFloat4(&c2);
  XMStoreFloat4(&result, XMColorModulate(C0, C1));
}

Color Color::Modulate(const Color& c1, const Color& c2)
{
  using namespace DirectX;
  XMVECTOR C0 = XMLoadFloat4(&c1);
  XMVECTOR C1 = XMLoadFloat4(&c2);

  Color result;
  XMStoreFloat4(&result, XMColorModulate(C0, C1));
  return result;
}

void Color::Lerp(const Color& c1, const Color& c2, float t, Color& result)
{
  using namespace DirectX;
  XMVECTOR C0 = XMLoadFloat4(&c1);
  XMVECTOR C1 = XMLoadFloat4(&c2);
  XMStoreFloat4(&result, XMVectorLerp(C0, C1, t));
}

Color Color::Lerp(const Color& c1, const Color& c2, float t)
{
  using namespace DirectX;
  XMVECTOR C0 = XMLoadFloat4(&c1);
  XMVECTOR C1 = XMLoadFloat4(&c2);

  Color result;
  XMStoreFloat4(&result, XMVectorLerp(C0, C1, t));
  return result;
}

}  // namespace LL3D
}  // namespace Math




