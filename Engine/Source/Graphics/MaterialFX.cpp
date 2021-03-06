#include "Graphics/MaterialFX.h"

LL3D::Graphics::MaterialFX::MaterialFX(const Material& mat) noexcept
{
  ambient = mat.ambient;
  diffuse = mat.diffuse;
  specular = mat.specular;
  emissive = mat.emissive;
  transparent = mat.transparent;
  shininess = mat.shininess;
  opacity = mat.opacity;
  shininessStrength = mat.shininess_strength;
  is_shadow = mat.shadow;
}
