#pragma once

#include <filesystem>
#include <wrl.h>
#include <DirectXMath.h>
#include <d3dx11effect.h>
#include "Core/Uncopyable.h"
#include "Graphics/LightFX.h"
#include "Graphics/MaterialFX.h"
#include "Graphics/Base.h"
#include "Graphics/Fog.h"

struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3DX11Effect;
struct ID3DX11EffectTechnique;
struct ID3DX11EffectPass;
struct ID3DX11EffectVariable;
struct ID3DX11EffectMatrixVariable;
struct ID3DX11EffectVectorVariable;
struct ID3DX11EffectShaderResourceVariable;
struct ID3D11ShaderResourceView;

namespace LL3D {
namespace Graphics {

class Effect : private Core::Uncopyable, protected Base {
public:
  Effect(std::string path);
  virtual ~Effect() {}

  virtual void apply(ID3D11DeviceContext* deviceContext) = 0;
  virtual void vertexShaderBytecode(const void ** pShaderByteCode, size_t* pByteCodeLength) = 0;

protected:
  Microsoft::WRL::ComPtr<ID3DX11Effect> effect_;
};

class BasicEffect : public Effect {
public:
  BasicEffect(std::string path);

  void apply(ID3D11DeviceContext* device_context) override;
  void vertexShaderBytecode(const void ** byte_code, size_t* byte_code_length) override;

  void setAmbientLight(const AmbientLightFX& value);
  void setDirectionalLight(const DirectionalLightFX& value);
  void setPointLight(const PointLightFX& value);
  void setSpotLight(const SpotLightFX& value);
  void setEyePosW(DirectX::FXMVECTOR value);

  void setWorld(DirectX::FXMMATRIX value);
  void setViewProjection(DirectX::FXMMATRIX value);

  void setTextureTransform(DirectX::FXMMATRIX value);
  void setDiffuseTex(ID3D11ShaderResourceView* value);
  void setNormalTex(ID3D11ShaderResourceView* value);

  void setMaterial(const MaterialFX& material);

  void setFog(const EffectFog& value);

private:
  ID3DX11EffectTechnique* tech_;

  // Per Frame:
  ID3DX11EffectVariable* ambientLight_;
  ID3DX11EffectVariable* directionalLight_;
  ID3DX11EffectVariable* pointLight_;
  ID3DX11EffectVariable* spotLight_;
  ID3DX11EffectMatrixVariable* viewProjection_;
  ID3DX11EffectVectorVariable* eyePosW_;
  ID3DX11EffectVariable* fog_;

  // Per Object:
  ID3DX11EffectMatrixVariable* world_;
  ID3DX11EffectVariable* material_;
  ID3DX11EffectMatrixVariable* textureTransform_;
  ID3DX11EffectScalarVariable* hasDiffuseTex2D_;
  ID3DX11EffectShaderResourceVariable* diffuseTex2D_;
  ID3DX11EffectScalarVariable* hasDiffuseTexCube_;
  ID3DX11EffectShaderResourceVariable* diffuseTexCube;
  ID3DX11EffectScalarVariable* hasNormalTex_;
  ID3DX11EffectShaderResourceVariable* normalTex_;
};

}  // namespace Graphics
}  // namespace LL3D