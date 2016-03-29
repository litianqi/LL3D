#pragma once

#include <filesystem>
#include "../Math/Math.h"

namespace LL3D {
namespace Graphics {

struct Material {
  std::string  name;
  
  Math::Vector3 ambient;
  Math::Vector3 diffuse;
  Math::Vector3 specular;
  Math::Vector3 emissive;  // todo
  Math::Vector3 transparent;  // todo: light passing through a transparent surface is multiplied by this filter color
  float shininess = 0.f;  // specular_exponent
  float opacity = 0.f;
  float shininess_strength = 0.f; // multiplier on top of specular color

  // Texture paths
  std::experimental::filesystem::path  diffuse_texture;
  std::experimental::filesystem::path  specular_texture;
  std::experimental::filesystem::path  emissive_texture;
  std::experimental::filesystem::path  normal_texture;
  std::experimental::filesystem::path  lightmap_texture;
  std::experimental::filesystem::path  reflection_texture;
};

}  // namespace Graphics
}  // namespace LL3D 

