#include "Light.h"
#include "Effects.h"

namespace LL3D {
namespace Graphics {

AmbientLight::AmbientLight(const Data & d) :
data(d) {
}

std::unique_ptr<Component> AmbientLight::Clone() {
  return std::unique_ptr<Component>(new AmbientLight(*this));
}

void AmbientLight::Update() {
  s_effect->SetAmbientLight(*this);
}

std::unique_ptr<Component> DirectionalLight::Clone() {
  return std::unique_ptr<Component>(new DirectionalLight(*this));
}

void DirectionalLight::Update() {
  s_effect->SetDirectionalLight(*this);
}

std::unique_ptr<Component> PointLight::Clone() {
  return std::unique_ptr<Component>(new PointLight(*this));
}

void PointLight::Update() {
  s_effect->SetPointLight(*this);
}

std::unique_ptr<Component> SpotLight::Clone() {
  return std::unique_ptr<Component>(new SpotLight(*this));
}

void SpotLight::Update() {
  s_effect->SetSpotLight(*this);
}

}  // namespace Graphics
}  // namespace LL3D


