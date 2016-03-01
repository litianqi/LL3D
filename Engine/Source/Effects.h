#pragma once

#include <filesystem>
#include <wrl.h>
#include <DirectXMath.h>
#include <d3dx11effect.h>
#include "Lights.h"
#include "Material.h"
#include "Core\Uncopyable.h"

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

using namespace DirectX;

class Effect : private Uncopyable {
public:
  Effect(ID3D11Device* device, std::string path);
  virtual ~Effect() {}

  virtual void Apply(ID3D11DeviceContext* deviceContext) =0;
  virtual void GetVertexShaderBytecode(const void ** pShaderByteCode, size_t* pByteCodeLength) = 0;

protected:
  Microsoft::WRL::ComPtr<ID3DX11Effect> effect_;
};

class BasicEffect : public Effect {
public:
  BasicEffect(ID3D11Device* device, std::string path);

  // Effect methods.
  void Apply(ID3D11DeviceContext* device_context) override;
  void GetVertexShaderBytecode(const void ** byte_code, size_t* byte_code_length) override;

  // Light settings.
  void SetLights(const Lights& lights);
  void SetEyePosW(FXMVECTOR value);

  // Camera settings.
  void SetWorld(FXMMATRIX value);
  void SetView(FXMMATRIX value);
  void SetProjection(FXMMATRIX value);

  // Texture setting.
  void SetTextureTransform(FXMMATRIX value);
  void SetTexture(ID3D11ShaderResourceView* value);
  
  // Material settings.
  void SetMaterial(const Material& value);

  //unsigned int GetPassNum() const;
  //ID3DX11EffectPass* GetPass(unsigned int index);

private:
  ID3DX11EffectTechnique* tech_;

  // Per Frame:
  ID3DX11EffectVariable* ambient_light_;
  ID3DX11EffectVariable* directional_light_;
  ID3DX11EffectVariable* point_light_;
  ID3DX11EffectVariable* spot_light_;
  ID3DX11EffectVectorVariable* eye_pos_w_;

  // Per Object:
  ID3DX11EffectMatrixVariable* world_;
  ID3DX11EffectMatrixVariable* view_;
  ID3DX11EffectMatrixVariable* projection_;

  ID3DX11EffectMatrixVariable* texture_transform_;
  ID3DX11EffectVariable* material_;
  ID3DX11EffectShaderResourceVariable* texture_;
};

}  // namespace LL3D