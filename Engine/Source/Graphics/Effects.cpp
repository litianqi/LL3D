#include "Graphics/Effects.h"
#include <fstream>
#include <D3D11.h>
#include <d3dx11effect.h>
#include "Core/Assert.h"
#include "Core/Exceptions.h"
#include "Utils/Unicode.h"
#include "Graphics/Device.h"

using namespace std::experimental;
using namespace DirectX;

namespace LL3D {
namespace Graphics {

Effect::Effect(std::string path) {
  std::ifstream fs(Utils::Convert(path), std::ios::binary);
  ASSERT(fs.is_open());

  auto size = filesystem::file_size(Utils::Convert(path));
  std::vector<char> content(static_cast<unsigned>(size));
  fs.read(content.data(), size);

  throwIfFailed(D3DX11CreateEffectFromMemory(content.data(), size,
    0, s_graphicsDevice->device(), &effect_));
}

BasicEffect::BasicEffect(std::string path) :
  Effect(path) {
  tech_ = effect_->GetTechniqueByName("Tech");

  ambientLight_ = effect_->GetVariableByName("g_ambient_light");
  directionalLight_ = effect_->GetVariableByName("g_directional_light");
  pointLight_ = effect_->GetVariableByName("g_point_light");
  spotLight_ = effect_->GetVariableByName("g_spot_light");
  eyePosW_ = effect_->GetVariableByName("g_eyePosWS")->AsVector();

  world_ = effect_->GetVariableByName("g_world")->AsMatrix();
  viewProjection_ = effect_->GetVariableByName("g_viewProj")->AsMatrix();
  textureTransform_ = effect_->GetVariableByName("g_texTransform")->AsMatrix();
  material_ = effect_->GetVariableByName("g_material");

  diffuseTex2D_ = effect_->GetVariableByName("g_diffuseTex2D")->AsShaderResource();
  hasDiffuseTex2D_ = effect_->GetVariableByName("g_hasDiffuseTex2D")->AsScalar();
  diffuseTexCube = effect_->GetVariableByName("g_diffuseTexCube")->AsShaderResource();
  hasDiffuseTexCube_ = effect_->GetVariableByName("g_hasDiffuseTexCube")->AsScalar();

  fog_ = effect_->GetVariableByName("g_fog");
}

void BasicEffect::apply(ID3D11DeviceContext* device_context) {
  tech_->GetPassByIndex(0)->Apply(0, device_context);
}

void BasicEffect::vertexShaderBytecode(const void ** bytecode, size_t* bytecodeLength) {
  D3DX11_PASS_DESC pass_desc;
  tech_->GetPassByIndex(0)->GetDesc(&pass_desc);
  *bytecode = pass_desc.pIAInputSignature;
  *bytecodeLength = pass_desc.IAInputSignatureSize;
}

void BasicEffect::setAmbientLight(const AmbientLightFX & value) {
  ambientLight_->SetRawValue(&value, 0, sizeof(value));
}

void BasicEffect::setDirectionalLight(const DirectionalLightFX & value) {
  directionalLight_->SetRawValue(&value, 0, sizeof(value));
}

void BasicEffect::setPointLight(const PointLightFX & value) {
  pointLight_->SetRawValue(&value, 0, sizeof(value));
}

void BasicEffect::setSpotLight(const SpotLightFX & value) {
  spotLight_->SetRawValue(&value, 0, sizeof(value));
}

void BasicEffect::setEyePosW(FXMVECTOR value) {
  eyePosW_->SetFloatVector(reinterpret_cast<const float*>(&value));
}

void BasicEffect::setWorld(FXMMATRIX value) {
  world_->SetMatrix(reinterpret_cast<const float*>(&(value)));
}

void BasicEffect::setViewProjection(FXMMATRIX value) {
  viewProjection_->SetMatrix(reinterpret_cast<const float*>(&(value)));
}

void BasicEffect::setTextureTransform(FXMMATRIX value) {
  textureTransform_->SetMatrix(reinterpret_cast<const float*>(&(value)));
}

void BasicEffect::setTexture(ID3D11ShaderResourceView* value) {
  
  diffuseTex2D_->SetResource(nullptr);
  hasDiffuseTex2D_->SetBool(false);
  diffuseTexCube->SetResource(nullptr);
  hasDiffuseTexCube_->SetBool(false);

  if (value) {
    D3D11_SHADER_RESOURCE_VIEW_DESC desc;
    value->GetDesc(&desc);
    if (desc.ViewDimension == D3D11_SRV_DIMENSION_TEXTURECUBE) {
      diffuseTexCube->SetResource(value);
      hasDiffuseTexCube_->SetBool(true);
    }
    else {
      diffuseTex2D_->SetResource(value);
      hasDiffuseTex2D_->SetBool(true);
    }
  }
}

void BasicEffect::setMaterial(const MaterialFX& material) {
  material_->SetRawValue(&material, 0, sizeof(material));
}

void BasicEffect::setFog(const EffectFog & value)
{
  fog_->SetRawValue(&value, 0, sizeof(value));
}

//unsigned int BasicEffect::GetPassNum() const {
//  D3DX11_TECHNIQUE_DESC tech_desc;
//  tech_->GetDesc(&tech_desc);
//  return tech_desc.Passes;
//}
//
//ID3DX11EffectPass * BasicEffect::GetPass(unsigned int index) {
//  return tech_->GetPassByIndex(index);
//}

}  // namespace Graphics
}  // namespace LL3D