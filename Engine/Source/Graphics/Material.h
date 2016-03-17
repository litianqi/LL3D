#pragma once

#include <DirectXMath.h>

namespace LL3D {
namespace Graphics {

struct Material {
  DirectX::XMVECTOR diffuse;
  DirectX::XMVECTOR specular;
  float spec_power;
  // No range! light reflected from material transfers with no range limitiation.
  DirectX::XMFLOAT3 _pad;
};

}  // namespace Graphics
}  // namespace LL3D 

