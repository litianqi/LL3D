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
  
  auto rotation = transform.GetRotation();
  auto m = Math::Matrix::CreateFromYawPitchRoll(rotation.y, rotation.x, rotation.z);
  direction = Math::Vector3::Transform(Math::Vector3::Forward, m);
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
  auto m = Math::Matrix::CreateFromYawPitchRoll(rotation.y, rotation.x, rotation.z);
  direction = Math::Vector3::Transform(Math::Vector3::Forward, m);
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

