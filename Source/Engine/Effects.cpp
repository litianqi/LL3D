#include "Effects.h"
#include <fstream>
#include <D3D11.h>
#include <d3dx11effect.h>
#include "Debug.h"

namespace LL3D {

Effect::Effect(ID3D11Device * device, filesystem::path file) {
  std::ifstream fs(file, std::ios::binary);
  ASSERT(fs.is_open());

  auto size = filesystem::file_size(file);
  std::vector<char> content(static_cast<unsigned>(size));
  fs.read(content.data(), size);

  HR(D3DX11CreateEffectFromMemory(content.data(), size,
    0, device, &effect_));
}

BasicEffect::BasicEffect(ID3D11Device * device, filesystem::path file) :
Effect(device, file) {
  tech_ = effect_->GetTechniqueByName("Tech");

  ambient_light_ = effect_->GetVariableByName("g_ambient_light");
  directional_light_ = effect_->GetVariableByName("g_directional_light");
  point_light_ = effect_->GetVariableByName("g_point_light");
  spot_light_ = effect_->GetVariableByName("g_spot_light");
  eye_pos_w_ = effect_->GetVariableByName("g_eye_pos_w")->AsVector();

  world_ = effect_->GetVariableByName("g_world")->AsMatrix();
  wvp_ = effect_->GetVariableByName("g_wvp")->AsMatrix();
  material_ = effect_->GetVariableByName("g_material");
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

void BasicEffect::SetEyePosW(FXMVECTOR position) {
  eye_pos_w_->SetFloatVector(reinterpret_cast<const float*>(&position));
}

void BasicEffect::SetWVPMatrix(FXMMATRIX matrix) {
  wvp_->SetMatrix(reinterpret_cast<const float*>(&(matrix)));
}

void BasicEffect::SetWorldMatrix(FXMMATRIX matrix) {
  world_->SetMatrix(reinterpret_cast<const float*>(&(matrix)));
}

void BasicEffect::SetMaterial(const Material & material) {
  material_->SetRawValue(&(material), 0, sizeof(material));
}

unsigned int BasicEffect::GetPassNum() const {
  D3DX11_TECHNIQUE_DESC tech_desc;
  tech_->GetDesc(&tech_desc);
  return tech_desc.Passes;
}

ID3DX11EffectPass * BasicEffect::GetPass(unsigned int index) {
  return tech_->GetPassByIndex(index);
}

}  // namespace LL3D