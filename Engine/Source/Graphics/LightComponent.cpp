#include "LightComponent.h"
#include "Core/Assert.h"
#include "Transform.h"
#include "GameObject.h"
#include "Graphics/Effects.h"

namespace LL3D {
namespace Graphics {

LightComponent::LightComponent(const Transform& transform, LightType type) :
  transform_(transform),
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
    spot.innerConeAngle = 0.05f;
    light_.spot = spot;
  }
  else {
    ASSERT(false && "Parameter not in range!");
  }
}

LightComponent::LightComponent(const Transform& transform, const AmbientLight & light) :
  transform_(transform),
  type_(Ambient)
{
  light_.ambient = light;
}

LightComponent::LightComponent(const Transform& transform, const DirectionalLight & light) :
  transform_(transform),
  type_(Directional)
{
  light_.directional = light;
}

LightComponent::LightComponent(const Transform& transform, const PointLight & light) :
  transform_(transform),
  type_(Point)
{
  light_.point = light;
}

LightComponent::LightComponent(const Transform& transform, const SpotLight & light) :
  transform_(transform),
  type_(Spot)
{
  light_.spot = light;
}

void LightComponent::writeToEffect() const
{
  if (type_ == Ambient) {
    s_effect->setAmbientLight(AmbientLightFX(light_.ambient));
  } else if (type_ == Directional) {
    s_effect->setDirectionalLight(DirectionalLightFX(light_.directional, transform_));
  } else if (type_ == Point) {
    s_effect->setPointLight(PointLightFX(light_.point, transform_));
  } else if (type_ == Spot) {
    s_effect->setSpotLight(SpotLightFX(light_.spot, transform_));
  } else {
    ASSERT(false && "Invalid type_");
  }
}

LightComponent::LightType LightComponent::lightType() const
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

