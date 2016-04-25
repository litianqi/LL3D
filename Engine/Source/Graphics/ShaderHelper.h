#ifdef __cplusplus

#pragma once

#include "Math/Math.h"

#define HLSL_SEMANTICS(semantics)

using float2 = LL3D::Math::Vector2;
using float3 = LL3D::Math::Vector3;
using float4 = LL3D::Math::Vector4;
using uint = unsigned int;

#else

#define HLSL_SEMANTICS(semantics) semantics

#endif