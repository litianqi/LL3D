#pragma once

#include "Math\Math.h"

namespace LL3D {

struct Transform {
  Math::Vector3 position;
  Math::Vector3 rotation;
  DirectX::XMFLOAT3 scale;

  operator Math::Matrix();
};

inline
Transform::operator Math::Matrix() {
  auto p = Math::Matrix::CreateTranslation(position);
  auto r = Math::Matrix::CreateFromYawPitchRoll(rotation.x, rotation.y, rotation.z);
  auto s = Math::Matrix::CreateScale(scale);
  return p * r * s;
}

}  // namespace LL3D 
