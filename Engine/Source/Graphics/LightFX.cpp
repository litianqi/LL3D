#include "Graphics/LightFX.h"
#include "Transform.h"

using namespace LL3D::Math;

namespace LL3D {
namespace Graphics {

SpotLightFX::SpotLightFX(const SpotLight & light, const Transform& transform) noexcept
{
  diffuse = light.diffuse;
  specular = light.specular;
  attenuation = light.attenuation;
  innerConeAngle = light.innerConeAngle;
  outerConeAngle = light.outerConeAngle;

  position = transform.position();
  direction = transform.upVec();
}

AmbientLightFX::AmbientLightFX(const AmbientLight & light) noexcept
{
  ambient = light.ambient;
}

DirectionalLightFX::DirectionalLightFX(const DirectionalLight & light, const Transform& transform) noexcept
{
  diffuse = light.diffuse;
  specular = light.specular;
  
  auto rotation = transform.rotation();
  direction = transform.upVec();
}

PointLightFX::PointLightFX(const PointLight & light, const Transform& transform) noexcept
{
  diffuse = light.diffuse;
  specular = light.specular;
  attenuation = light.attenuation;
  
  position = transform.position();
}

}  // namespace Graphics
}  // namespace LL3D

