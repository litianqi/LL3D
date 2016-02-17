#pragma once

#include <DirectXMath.h>

namespace LLEngine{

DirectX::XMVECTOR operator+(const DirectX::XMVECTOR& l, const DirectX::XMVECTOR& r);
DirectX::XMVECTOR& operator+=(DirectX::XMVECTOR& l, const DirectX::XMVECTOR& r);

DirectX::XMVECTOR operator-(const DirectX::XMVECTOR& l, const DirectX::XMVECTOR& r);
DirectX::XMVECTOR& operator-=(DirectX::XMVECTOR& l, const DirectX::XMVECTOR& r);

DirectX::XMVECTOR operator*(const DirectX::XMVECTOR& l, const DirectX::XMVECTOR& r);
DirectX::XMVECTOR& operator*=(DirectX::XMVECTOR& l, const DirectX::XMVECTOR& r);

DirectX::XMVECTOR operator*(DirectX::XMVECTOR l, float r);

DirectX::XMVECTOR operator*(float l, DirectX::XMVECTOR r);

void Normalize(DirectX::XMVECTOR& v);

}  // namespace LLEngine