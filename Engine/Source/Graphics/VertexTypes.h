#pragma once

#include "Math/Math.h"

namespace LL3D {
namespace Graphics {

struct Vertex {
  Math::Vector3 position;
  Math::Vector3 normal;
  Math::Vector2 texcoord;
  Math::Vector3 tangent;  // todo
  Math::Vector3 bitangent;  // todo

  static const int kInputElementCount = 5;
  static const D3D11_INPUT_ELEMENT_DESC kInputElements[kInputElementCount];
};

}  // namespace Graphics
}  // namespace LL3D