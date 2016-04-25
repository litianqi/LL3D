#include "TextureHelper.h"
#include "Core/Exceptions.h"
#include "Math/Math.h"

Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>
LL3D::Graphics::CreateRandomTexture1D(ID3D11Device* device, unsigned size)
{
  using namespace Microsoft::WRL;

  std::vector<Math::Vector4> randomValues;
  
  randomValues.resize(size);
  for (unsigned i = 0; i < size; ++i) {
    randomValues[i].x = Math::randf(-1.0f, 1.0f);
    randomValues[i].y = Math::randf(-1.0f, 1.0f);
    randomValues[i].z = Math::randf(-1.0f, 1.0f);
    randomValues[i].w = Math::randf(-1.0f, 1.0f);
  }

  D3D11_SUBRESOURCE_DATA initData;
  initData.pSysMem = randomValues.data();
  initData.SysMemPitch = size * sizeof(Math::Vector4);
  initData.SysMemSlicePitch = 0;

  D3D11_TEXTURE1D_DESC texDesc;
  texDesc.Width = size;
  texDesc.MipLevels = 1;
  texDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
  texDesc.Usage = D3D11_USAGE_IMMUTABLE;
  texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
  texDesc.CPUAccessFlags = 0;
  texDesc.MiscFlags = 0;
  texDesc.ArraySize = 1;

  ComPtr<ID3D11Texture1D> randomTex;
  throwIfFailed(device->CreateTexture1D(&texDesc, &initData, &randomTex));

  D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;
  viewDesc.Format = texDesc.Format;
  viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE1D;
  viewDesc.Texture1D.MipLevels = texDesc.MipLevels;
  viewDesc.Texture1D.MostDetailedMip = 0;

  ComPtr<ID3D11ShaderResourceView> randomTexSRV;
  throwIfFailed(device->CreateShaderResourceView(randomTex.Get(), &viewDesc,
                                                 &randomTexSRV));

  return randomTexSRV;
}
