#pragma once

#include "../Math/Math.h"

namespace LL3D {
namespace Graphics {

struct Material {
  Math::Color diffuse;
  Math::Color specular;
  float       spec_power;
  // No range! light reflected from material transfers with no range limitiation.
  Math::Vector3 _pad;
};

}  // namespace Graphics
}  // namespace LL3D 

