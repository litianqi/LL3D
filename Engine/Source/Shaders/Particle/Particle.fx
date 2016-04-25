#include "../../Graphics/Particle/ParticleVertex.h"
#include "../CommonStates.fx"

cbuffer __perFrame__
{
  float3 g_emitPosWS;
  float3 g_emitDirWS;
  float3 g_eyePosWS;
  /*float g_accelWS;*/
  float4x4 g_viewProj;
  float g_randomNumber; /* scope: [-1, 1] */
  float g_timeStep;     /* unit: s */
};

Texture1D g_randomTex;
Texture2D g_colorTex;

/**
 * Returns a random normalized float3.
 */
float3
randf3(float offset)
{
  float u = g_randomNumber + offset;
  float3 v = g_randomTex.SampleLevel(kSamLinear, u, 0).xyz;
  return normalize(v);
}