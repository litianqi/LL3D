#include "LightComponent.h"
#include "..\Core\Assert.h"
#include "Effects.h"
#include "..\Transform.h"
#include "..\GameObject.h"

namespace LL3D {
namespace Graphics {

LightComponent::LightComponent(LightType type) :
  type_(type)
{
  if (type == Ambient) {
    auto ambient = AmbientLight();
    ambient.ambient = Math::Vector3(0.2f, 0.2f, 0.2f);
    light_.ambient = ambient;
  }
  else if (type == Directional) {
    auto directional = DirectionalLight();
    directional.diffuse = Math::Vector3(0.2f, 0.2f, 0.2f);
    directional.specular = Math::Vector3(0.2f, 0.2f, 0.2f);
    light_.directional = directional;
  }
  else if (type == Point) {
    auto point = PointLight();
    point.diffuse = Math::Color::Blue.ToVector3();
    point.specular = Math::Vector3(0.2f, 0.2f, 0.2f);
    point.attenuation = Math::Vector3(0.05f, 0.05f, 0.05f);
    light_.point = point;
  }
  else if (type == Spot) {
    auto spot = SpotLight();
    spot.diffuse = Math::Color::Yellow.ToVector3();
    spot.specular = Math::Vector3(0.2f, 0.2f, 0.2f);
    spot.attenuation = Math::Vector3(0.02f, 0.02f, 0.02f);
    spot.inner_cone_angle = 0.05f;
    light_.spot = spot;
  }
  else {
    ASSERT(false && "Parameter not in range!");
  }
}

LightComponent::LightComponent(const AmbientLight & light) :
  type_(Ambient)
{
  light_.ambient = light;
}

LightComponent::LightComponent(const DirectionalLight & light) :
  type_(Directional)
{
  light_.directional = light;
}

LightComponent::LightComponent(const PointLight & light) :
  type_(Point)
{
  light_.point = light;
}

LightComponent::LightComponent(const SpotLight & light) :
  type_(Spot)
{
  light_.spot = light;
}

std::unique_ptr<Component> LightComponent::Clone()
{
  return std::make_unique<LightComponent>(*this);
}

void LightComponent::Render() const
{
  const auto& transform = GetGameObject()->GetTransform();

  if (type_ == Ambient) {
    s_effect->SetAmbientLight(AmbientLightFX(light_.ambient));
  } else if (type_ == Directional) {
    s_effect->SetDirectionalLight(DirectionalLightFX(light_.directional, transform));
  } else if (type_ == Point) {
    s_effect->SetPointLight(PointLightFX(light_.point, transform));
  } else if (type_ == Spot) {
    s_effect->SetSpotLight(SpotLightFX(light_.spot, transform));
  } else {
    ASSERT(false && "Invalid type_");
  }
}

LightComponent::LightType LightComponent::GetLightType() const
{
  return type_;
}

LightComponent::Light::Light() noexcept {
  std::memset(this, 0, sizeof(*this));
}
LightComponent::Light::Light(const Light& rhs) noexcept {
  std::memcpy(this, &rhs, sizeof(*this));
}

LightComponent::Light& LightComponent::Light::operator=(const Light& rhs) noexcept {
  Light tmp(rhs);
  std::swap(*this, tmp);
  return *this;
}

LightComponent::Light::~Light() noexcept {
  // Do nothing.
}

}  // namespace Graphics
}  // namespace LL3D

