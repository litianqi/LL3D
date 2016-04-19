#ifdef __cplusplus
#pragma once

#include "Math/Math.h"
#include "Graphics/Material.h"

using float2 = LL3D::Math::Vector2;
using float3 = LL3D::Math::Vector3;
using float4 = LL3D::Math::Vector4;

namespace LL3D
{
  namespace Graphics
  {
  #endif // __cplusplus

    struct MaterialFX
    {

    #ifdef __cplusplus
      MaterialFX(const Material& mat) noexcept;
    #endif // __cplusplus

      float3 ambient;
      float _pad0;

      float3 diffuse;
      float _pad1;

      float3 specular;
      float _pad2;

      float3 emissive;
      float _pad3;

      float3 transparent;
      float _pad4;

      float shininess;
      float opacity;
      float shininessStrength;
      int   is_shadow;
    };

  #ifdef __cplusplus
  }  // namespace LL3D
}  // namespace Graphics
#endif // __cplusplus