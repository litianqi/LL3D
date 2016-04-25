#include "Graphics/Particle/ParticleVertex.h"

namespace LL3D {
namespace Graphics {

const D3D11_INPUT_ELEMENT_DESC ParticleVertex::kInputElements[] = {
  { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT,
  D3D11_INPUT_PER_VERTEX_DATA, 0 },
  { "VELOCITY", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT,
  D3D11_INPUT_PER_VERTEX_DATA, 0 },
  { "SIZE", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT,
  D3D11_INPUT_PER_VERTEX_DATA, 0 },
  { "AGE", 0, DXGI_FORMAT_R32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT,
  D3D11_INPUT_PER_VERTEX_DATA, 0 },
  { "FLAG", 0, DXGI_FORMAT_R32_UINT, 0, D3D11_APPEND_ALIGNED_ELEMENT,
  D3D11_INPUT_PER_VERTEX_DATA, 0 }
};

static_assert(sizeof(ParticleVertex) == 4 * 3 + 4 * 3 + 4 * 2 + 4 + 4,
              "Vertex struct/layout mismatch");

} // namespace Graphics
} // namespace LL3D