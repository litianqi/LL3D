#include "ParticleEffect.h"
#include <filesystem>
#include "Graphics/Effects.h"
#include "Math/Math.h"
#include "Graphics/Particle/ParticleEffect.h"
#include "Core/Assert.h"
#include "Core/Exceptions.h"

namespace LL3D {
namespace Graphics {

ParticleEffect::ParticleEffect(std::string pathname)
  : Effect(pathname)
{
  streamOutTech_ = effect_->GetTechniqueByName("StreamOutTech");
  drawTech_ = effect_->GetTechniqueByName("DrawTech");

  viewProj_ = effect_->GetVariableByName("g_viewProj")->AsMatrix();
  timeStep_ = effect_->GetVariableByName("g_timeStep")->AsScalar();
  eyePosWS_ = effect_->GetVariableByName("g_eyePosWS")->AsVector();
  emitPosWS_ = effect_->GetVariableByName("g_emitPosWS")->AsVector();
  emitDirWS_ = effect_->GetVariableByName("g_emitDirWS")->AsVector();
  colorTex_ = effect_->GetVariableByName("g_colorTex")->AsShaderResource();
  randomNumber_ = effect_->GetVariableByName("g_randomNumber")->AsScalar();
  randomTex_ = effect_->GetVariableByName("g_randomTex")->AsShaderResource();
}

void
ParticleEffect::vertexShaderBytecode(const void** bytecode,
                                     size_t* bytecodeLength)
{
  D3DX11_PASS_DESC pass_desc;
  streamOutTech_->GetPassByIndex(0)->GetDesc(&pass_desc);
  *bytecode = pass_desc.pIAInputSignature;
  *bytecodeLength = pass_desc.IAInputSignatureSize;
}

void
ParticleEffect::apply(Technique tech, ID3D11DeviceContext* dc)
{
  if (tech == kSwapOut)
    throwIfFailed(streamOutTech_->GetPassByIndex(0)->Apply(0, dc));
  else if (tech == kDraw)
    throwIfFailed(drawTech_->GetPassByIndex(0)->Apply(0, dc));
  else
    ASSERT(false);
}

void
ParticleEffect::setViewProj(const Math::Matrix& value)
{
  viewProj_->SetMatrix(reinterpret_cast<const float*>(&value));
}

void
ParticleEffect::setTimeStep(float value)
{
  timeStep_->SetFloat(value);
}

void
ParticleEffect::setEyePosWS(Math::Vector3 value)
{
  eyePosWS_->SetFloatVector(reinterpret_cast<const float*>(&value));
}

void
ParticleEffect::setEmitPosWS(Math::Vector3 value)
{
  emitPosWS_->SetFloatVector(reinterpret_cast<const float*>(&value));
}

void
ParticleEffect::setEmitDirWS(Math::Vector3 value)
{
  emitDirWS_->SetFloatVector(reinterpret_cast<const float*>(&value));
}

void
ParticleEffect::setColorTex(ID3D11ShaderResourceView* value)
{
  colorTex_->SetResource(value);
}

void
ParticleEffect::setRandomNumber(float value)
{
  randomNumber_->SetFloat(value);
}

void
ParticleEffect::setRandomTex(ID3D11ShaderResourceView* value)
{
  randomTex_->SetResource(value);
}

} // namespace Graphics
} // namespace LL3D