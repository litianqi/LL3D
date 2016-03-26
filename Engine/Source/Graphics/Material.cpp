#include "Material.h"

LL3D::Graphics::Material::operator EffectBuffer() const
{
  auto buffer = EffectBuffer();
  buffer.ambient = ambient;
  buffer.diffuse = diffuse;
  buffer.specular = specular;
  buffer.emissive = emissive;
  buffer.transparent = transparent;
  buffer.shininess = shininess;
  buffer.opacity = opacity;
  buffer.shininess_strength = shininess_strength;

  return buffer;
}
