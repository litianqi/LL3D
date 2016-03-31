#include "LightFX.h"
#include "../Transform.h"

using namespace LL3D::Math;

namespace LL3D {
namespace Graphics {

SpotLightFX::SpotLightFX(const SpotLight & light, const Transform& transform) noexcept
{
  diffuse = light.diffuse;
  specular = light.specular;
  attenuation = light.attenuation;
  inner_cone_angle = light.inner_cone_angle;
  outer_cone_angle = light.outer_cone_angle;

  position = transform.GetPosition();
  direction = transform.GetDirection();
}

AmbientLightFX::AmbientLightFX(const AmbientLight & light) noexcept
{
  ambient = light.ambient;
}

DirectionalLightFX::DirectionalLightFX(const DirectionalLight & light, const Transform& transform) noexcept
{
  diffuse = light.diffuse;
  specular = light.specular;
  
  auto rotation = transform.GetRotation();
  direction = transform.GetDirection();
}

PointLightFX::PointLightFX(const PointLight & light, const Transform& transform) noexcept
{
  diffuse = light.diffuse;
  specular = light.specular;
  attenuation = light.attenuation;
  
  position = transform.GetPosition();
}

}  // namespace Graphics
}  // namespace LL3D

