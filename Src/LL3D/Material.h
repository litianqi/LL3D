#pragma once

#include <DirectXMath.h>

namespace LL3D {

using namespace DirectX;

struct Material {
  XMVECTOR ambient;
  XMVECTOR diffuse;
  XMVECTOR specular;
  float spec_power;
  // No range! light reflected from material transfers with no range limitiation.
  XMFLOAT3 _pad;
};

}  // namespace LL3D 

