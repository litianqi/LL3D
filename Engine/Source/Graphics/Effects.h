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

class Effect : private Uncopyable, protected Base {
public:
  Effect(std::string path);
  virtual ~Effect() {}

  virtual void Apply(ID3D11DeviceContext* deviceContext) = 0;
  virtual void GetVertexShaderBytecode(const void ** pShaderByteCode, size_t* pByteCodeLength) = 0;

protected:
  Microsoft::WRL::ComPtr<ID3DX11Effect> effect_;
};

class BasicEffect : public Effect {
public:
  BasicEffect(std::string path);

  //--------------------------------------
  // Effect methods.
  void Apply(ID3D11DeviceContext* device_context) override;
  void GetVertexShaderBytecode(const void ** byte_code, size_t* byte_code_length) override;

  //--------------------------------------
  // Light settings.
  void SetAmbientLight(const AmbientLightFX& value);
  void SetDirectionalLight(const DirectionalLightFX& value);
  void SetPointLight(const PointLightFX& value);
  void SetSpotLight(const SpotLightFX& value);
  void SetEyePosW(DirectX::FXMVECTOR value);

  //--------------------------------------
  // Camera settings.
  void SetWorld(DirectX::FXMMATRIX value);
  void SetViewProjection(DirectX::FXMMATRIX value);

  //--------------------------------------
  // Texture setting.
  void SetTextureTransform(DirectX::FXMMATRIX value);
  void SetTexture(ID3D11ShaderResourceView* value);

  //--------------------------------------
  // Material settings.
  void SetMaterial(const MaterialFX& material);

  //--------------------------------------
  // Fog settings.
  void SetFog(const EffectFog& value);

private:
  ID3DX11EffectTechnique* tech_;

  // Per Frame:
  ID3DX11EffectVariable* ambient_light_;
  ID3DX11EffectVariable* directional_light_;
  ID3DX11EffectVariable* point_light_;
  ID3DX11EffectVariable* spot_light_;
  ID3DX11EffectMatrixVariable* view_projection_;
  ID3DX11EffectVectorVariable* eye_pos_w_;
  ID3DX11EffectVariable* fog_;

  // Per Object:
  ID3DX11EffectMatrixVariable* world_;
  ID3DX11EffectVariable* material_;
  ID3DX11EffectShaderResourceVariable* texture_;
  ID3DX11EffectMatrixVariable* texture_transform_;
};

}  // namespace Graphics
}  // namespace LL3D