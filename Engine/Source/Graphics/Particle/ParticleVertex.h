#include "../../Graphics/ShaderHelper.h"

#ifdef __cplusplus
#pragma once

#include "Math/Math.h"

namespace LL3D {
namespace Graphics {
#endif // __cplusplus

struct ParticleVertex
{
  float3 posWS HLSL_SEMANTICS(: POSITION);
  float3 velWS HLSL_SEMANTICS(: VELOCITY);
  float2 sizeWS HLSL_SEMANTICS(: SIZE);
  float age HLSL_SEMANTICS(: AGE); /* unit: s */
  /**
   * Reserved for user, initial value is 0. 
   * Usually 0 is used as none, 1 is used as created.
   */ 
  uint flag HLSL_SEMANTICS(: FLAG); 

#ifdef __cplusplus
  static const int kInputElementCount = 5;
  static const D3D11_INPUT_ELEMENT_DESC kInputElements[kInputElementCount];
#endif // __cplusplus
};

#ifdef __cplusplus
} // namespace Graphics
} // namespace LL3D
#endif // __cplusplus