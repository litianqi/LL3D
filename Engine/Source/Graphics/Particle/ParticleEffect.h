#pragma once

#include <filesystem>
#include "Graphics/Effects.h"
#include "Math/Math.h"

namespace LL3D {
namespace Graphics {

class ParticleEffect : public Effect
{
public:
  enum Technique
  {
    kSwapOut,
    kDraw
  };

  ParticleEffect(std::string pathname);

  void apply(ID3D11DeviceContext* deviceContext) override {}
  void vertexShaderBytecode(const void** bytecode,
                            size_t* bytecodeLength) override;
  void apply(Technique tech, ID3D11DeviceContext* dc);

  void setViewProj(const Math::Matrix& value);

  void setTimeStep(float value);

  void setEyePosWS(Math::Vector3 value);
  void setEmitPosWS(Math::Vector3 value);
  void setEmitDirWS(Math::Vector3 value);

  void setColorTex(ID3D11ShaderResourceView* value);

  void setRandomNumber(float value);
  void setRandomTex(ID3D11ShaderResourceView* value);

private:
  ID3DX11EffectTechnique* streamOutTech_;
  ID3DX11EffectTechnique* drawTech_;

  ID3DX11EffectMatrixVariable* viewProj_;
  ID3DX11EffectScalarVariable* randomNumber_;
  ID3DX11EffectScalarVariable* timeStep_;
  ID3DX11EffectVectorVariable* eyePosWS_;
  ID3DX11EffectVectorVariable* emitPosWS_;
  ID3DX11EffectVectorVariable* emitDirWS_;
  ID3DX11EffectShaderResourceVariable* colorTex_;
  ID3DX11EffectShaderResourceVariable* randomTex_;
};

} // namespace Graphics
} // namespace LL3D