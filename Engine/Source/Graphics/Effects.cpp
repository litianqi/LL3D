#include "Effects.h"
#include <fstream>
#include <D3D11.h>
#include <d3dx11effect.h>
#include "../Core/Assert.h"
#include "../Core/Exceptions.h"
#include "../Utils/Unicode.h"
#include "Device.h"

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

  ThrowIfFailed(D3DX11CreateEffectFromMemory(content.data(), size,
    0, s_graphics_device->GetDevice(), &effect_));
}

BasicEffect::BasicEffect(std::string path) :
  Effect(path) {
  tech_ = effect_->GetTechniqueByName("Tech");

  ambient_light_ = effect_->GetVariableByName("g_ambient_light");
  directional_light_ = effect_->GetVariableByName("g_directional_light");
  point_light_ = effect_->GetVariableByName("g_point_light");
  spot_light_ = effect_->GetVariableByName("g_spot_light");
  eye_pos_w_ = effect_->GetVariableByName("g_eye_pos_w")->AsVector();

  world_ = effect_->GetVariableByName("g_world")->AsMatrix();
  view_ = effect_->GetVariableByName("g_view")->AsMatrix();
  projection_ = effect_->GetVariableByName("g_projection")->AsMatrix();
  texture_transform_ = effect_->GetVariableByName("g_texture_transform")->AsMatrix();
  material_ = effect_->GetVariableByName("g_material");

  texture_ = effect_->GetVariableByName("g_texture")->AsShaderResource();

  fog_ = effect_->GetVariableByName("g_fog");
}

void BasicEffect::Apply(ID3D11DeviceContext* device_context) {
  tech_->GetPassByIndex(0)->Apply(0, device_context);
}

void BasicEffect::GetVertexShaderBytecode(const void ** byte_code, size_t* byte_code_length) {
  D3DX11_PASS_DESC pass_desc;
  tech_->GetPassByIndex(0)->GetDesc(&pass_desc);
  *byte_code = pass_desc.pIAInputSignature;
  *byte_code_length = pass_desc.IAInputSignatureSize;
}

void BasicEffect::SetAmbientLight(const AmbientLightFX & value) {
  ambient_light_->SetRawValue(&value, 0, sizeof(value));
}

void BasicEffect::SetDirectionalLight(const DirectionalLightFX & value) {
  directional_light_->SetRawValue(&value, 0, sizeof(value));
}

void BasicEffect::SetPointLight(const PointLightFX & value) {
  point_light_->SetRawValue(&value, 0, sizeof(value));
}

void BasicEffect::SetSpotLight(const SpotLightFX & value) {
  spot_light_->SetRawValue(&value, 0, sizeof(value));
}

void BasicEffect::SetEyePosW(FXMVECTOR value) {
  eye_pos_w_->SetFloatVector(reinterpret_cast<const float*>(&value));
}

void BasicEffect::SetWorld(FXMMATRIX value) {
  world_->SetMatrix(reinterpret_cast<const float*>(&(value)));
}

void BasicEffect::SetView(FXMMATRIX value) {
  view_->SetMatrix(reinterpret_cast<const float*>(&(value)));
}

void BasicEffect::SetProjection(FXMMATRIX value) {
  projection_->SetMatrix(reinterpret_cast<const float*>(&(value)));
}

void BasicEffect::SetTextureTransform(FXMMATRIX value) {
  texture_transform_->SetMatrix(reinterpret_cast<const float*>(&(value)));
}

void BasicEffect::SetTexture(ID3D11ShaderResourceView* value) {
  texture_->SetResource(value);
}

void BasicEffect::SetMaterial(const MaterialFX& material) {
  material_->SetRawValue(&material, 0, sizeof(material));
}

void BasicEffect::SetFog(const EffectFog & value)
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