#pragma once

#include "Math/Math.h"

namespace LL3D {
namespace Graphics {

//! A spot light source emits light in a specific
//! angle. It has a position and a direction it is pointing to.
//! A good example for a spot light is a light spot in
//! sport arenas.
struct SpotLight {
  std::string name;

  /** Diffuse color of the light source
  *
  *  The diffuse light color is multiplied with the diffuse
  *  material color to obtain the final color that contributes
  *  to the diffuse shading term.
  */
  Math::Vector3 diffuse;

  /** Specular color of the light source
  *
  *  The specular light color is multiplied with the specular
  *  material color to obtain the final color that contributes
  *  to the specular shading term.
  */
  Math::Vector3 specular;

  /** Light attenuation factor.
  *
  *  The intensity of the light source at a given distance 'd' from
  *  the light's position is
  *  @code
  *  Atten = 1/( att0 + att1 * d + att2 * d*d)
  *  @endcode
  */
  Math::Vector3 attenuation;

  /** Inner angle of a spot light's light cone.
  *
  *  The spot light has maximum influence on objects inside this
  *  angle. The angle is given in radians.
  */
  float inner_cone_angle;  // todo: currently use this as cone factor.

  /** Outer angle of a spot light's light cone.
  *
  *  The spot light does not affect objects outside this angle.
  *  The angle is given in radians. The outer angle must be
  *  greater than or equal to the inner angle.
  *  It is assumed that the application uses a smooth
  *  interpolation between the inner and the outer cone of the
  *  spot light.
  */
  float outer_cone_angle;  // todo
};

//! The generic light level of the world, including the bounces
//! of all other lightsources.
//! Typically, there's at most one ambient light in a scene.
//! This light type doesn't have a valid position, direction, or
//! other properties, just a color.
struct AmbientLight {
  std::string name;
  Math::Vector3 ambient;
};

//! A directional light source has a well-defined direction
//! but is infinitely far away. That's quite a good
//! approximation for sun light.
struct DirectionalLight {
  std::string name;
  Math::Vector3 diffuse;
  Math::Vector3 specular;
};

//! A point light source has a well-defined position
//! in space but no direction - it emits light in all
//! directions. A normal bulb is a point light.
struct PointLight {
  std::string name;
  Math::Vector3 diffuse;
  Math::Vector3 specular;
  Math::Vector3 attenuation;
};

}  // namespace Graphics
}  // namespace LL3D 

