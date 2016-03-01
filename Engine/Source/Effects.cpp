#include "Effects.h"
#include <fstream>
#include <D3D11.h>
#include <d3dx11effect.h>
#include "Core/Assert.h"
#include "Core/Exceptions.h"
#include "Core/Encoding.h"

using namespace std::experimental;

namespace LL3D {

Effect::Effect(ID3D11Device * device, std::string path) {
  std::ifstream fs(Utils::Convert(path), std::ios::binary);
  ASSERT(fs.is_open());

  auto size = filesystem::file_size(Utils::Convert(path));
  std::vector<char> content(static_cast<unsigned>(size));
  fs.read(content.data(), size);

  ThrowIfFailed(D3DX11CreateEffectFromMemory(content.data(), size,
    0, device, &effect_));
}

BasicEffect::BasicEffect(ID3D11Device * device, std::string path) :
Effect(device, path) {
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

void BasicEffect::SetLights(const Lights & lights) {
  for (auto light : lights.ambients) {
    ambient_light_->SetRawValue(&light, 0, sizeof(AmbientLight));
  }
  for (auto light : lights.directionals) {
    directional_light_->SetRawValue(&light, 0, sizeof(DirectionalLight));
  }
  for (auto light : lights.points) {
    point_light_->SetRawValue(&light, 0, sizeof(PointLight));
  }
  for (auto light : lights.spots) {
    spot_light_->SetRawValue(&light, 0, sizeof(SpotLight));
  }
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

void BasicEffect::SetMaterial(const Material & value) {
  material_->SetRawValue(&(value), 0, sizeof(value));
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

}  // namespace LL3D