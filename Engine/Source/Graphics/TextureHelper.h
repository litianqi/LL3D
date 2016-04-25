#pragma once

#include <vector>
#include <wrl.h>
#include <d3d11.h>

struct ID3D11ShaderResourceView;

namespace LL3D {
namespace Graphics {

Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> CreateRandomTexture1D(
  ID3D11Device* device, unsigned size = 1024u);

} // namespace Graphics
} // namespace LL3D