#pragma once

#include <filesystem>
#include <DirectXMath.h>
#include "Lights.h"
#include "Material.h"

struct ID3D11Device;
struct ID3DX11Effect;
struct ID3DX11EffectTechnique;
struct ID3DX11EffectPass;
struct ID3DX11EffectVariable;
struct ID3DX11EffectMatrixVariable;
struct ID3DX11EffectVectorVariable;

namespace LL3D {

namespace filesystem = std::experimental::filesystem;
using namespace DirectX;

class Effect {
public:
  Effect(ID3D11Device* device, filesystem::path file);

protected:
  ID3DX11Effect* effect_;
};

class BasicEffect : public Effect {
public:
  BasicEffect(ID3D11Device* device, filesystem::path file);

  // Per Frame:
  void SetLights(const Lights& lights);
  void SetEyePosW(FXMVECTOR position);

  // Per Object:
  void SetWVPMatrix(FXMMATRIX matrix);
  void SetWorldMatrix(FXMMATRIX matrix);
  void SetMaterial(const Material& material);

  unsigned int GetPassNum() const;
  ID3DX11EffectPass* GetPass(unsigned int index);

private:
  ID3DX11EffectTechnique* tech_;

  // Per Frame:
  ID3DX11EffectVariable* ambient_light_;
  ID3DX11EffectVariable* directional_light_;
  ID3DX11EffectVariable* point_light_;
  ID3DX11EffectVariable* spot_light_;
  ID3DX11EffectVectorVariable* eye_pos_w_;

  // Per Object:
  ID3DX11EffectMatrixVariable* wvp_;
  ID3DX11EffectMatrixVariable* world_;
  ID3DX11EffectVariable* material_;
};

}