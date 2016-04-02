#ifdef __cplusplus
#pragma once

#include "../Math/Math.h"
#include "Light.h"

using float2 = LL3D::Math::Vector2;
using float3 = LL3D::Math::Vector3;
using float4 = LL3D::Math::Vector4;

namespace LL3D {

class Transform;

namespace Graphics {
#endif // __cplusplus

struct SpotLightFX {
#ifdef __cplusplus
  SpotLightFX(const SpotLight& light, const Transform& transform) noexcept;
#endif // __cplusplus

  float3  diffuse;
  float   _pad0;

  float3  specular;
  float   _pad1;

  float3  direction;
  float   _pad2;

  float3  position;
  float   _pad3;

  float3  attenuation;
  float   _pad4;

  float   inner_cone_angle;
  float   outer_cone_angle;
  float2  _pad5;
};

struct AmbientLightFX {
#ifdef __cplusplus
  AmbientLightFX(const AmbientLight& light) noexcept;
#endif // __cplusplus

  float3  ambient;
  float   _pad0;
};

struct DirectionalLightFX {
#ifdef __cplusplus
  DirectionalLightFX(const DirectionalLight& light, const Transform& transform) noexcept;
#endif // __cplusplus

  float3  diffuse;
  float   _pad0;

  float3  specular;
  float   _pad1;

  float3  direction;
  float   _pad2;
};

struct PointLightFX {
#ifdef __cplusplus
  PointLightFX(const PointLight& light, const Transform& transform) noexcept;
#endif // __cplusplus

  float3  diffuse;
  float   _pad0;

  float3  specular;
  float   _pad1;

  float3  position;
  float   _pad2;

  float3  attenuation;
  float   _pad3;
};

#ifdef __cplusplus
}  // namespace LL3D
}  // namespace Graphics
#endif // __cplusplus