#include "Particle.fx"

cbuffer __constants__
{
  uint kNone = 0;
  uint kUsing = 1;
  float3 kAccelWS = { 0.0f, 7.8f, 0.0f };
  float2 kQuadTexCoords[4] = { float2(0.0f, 1.0f), float2(1.0f, 1.0f),
                               float2(0.0f, 0.0f), float2(1.0f, 0.0f) };
};

/**
 * Simplely pass by.
 */
ParticleVertex
StreamOutVS(ParticleVertex vin)
{
  return vin;
}

/**
 * Creates or destorys particle.
 */
[maxvertexcount(2)] void
StreamOutGS(point ParticleVertex gin[1], inout PointStream<ParticleVertex> gout)
{
  gin[0].age += g_timeStep;

  if (gin[0].flag == kNone) {
    ParticleVertex pv;

    float3 velWS = randf3(0.f);
    velWS.x *= 0.5f;
    velWS.y *= 0.5f;
    pv.velWS = 44.f * velWS;

    pv.posWS = g_emitPosWS;
    pv.sizeWS = float2(3.0f, 3.0f);
    pv.age = 0;
    pv.flag = kUsing;

    gout.Append(pv);
    gout.Append(gin[0]);
  } else {
    if (gin[0].age < 1.f) {
      gout.Append(gin[0]);
    }
  }
}

GeometryShader CompiledStreamOutGS =
  ConstructGSWithSO(CompileShader(gs_5_0, StreamOutGS()),
                    "POSITION.xyz; VELOCITY.xyz; SIZE.xy; AGE.x; FLAG.x");

technique11 StreamOutTech
{
  pass p0
  {
    SetVertexShader(CompileShader(vs_5_0, StreamOutVS()));
    SetHullShader(NULL);
    SetDomainShader(NULL);
    SetGeometryShader(CompiledStreamOutGS);
    SetPixelShader(NULL);
    SetDepthStencilState(kDisableDepth, 0);
  }
}

struct VertexOut
{
  float3 posWS : POSITION;
  float2 sizeWS : SIZE;
  float4 color : COLOR;
  uint flag : FLAG;
};

/**
 * Calculates position and alpha.
 */
VertexOut
DrawVS(ParticleVertex vin)
{
  VertexOut vout;

  vout.sizeWS = vin.sizeWS;
  vout.flag = vin.flag;

  float t = vin.age;
  vout.posWS = 0.5f * kAccelWS * t * t + vin.velWS * t + vin.posWS;

  float a = 1.f - clamp(vin.age, 0.f, 1.f);
  vout.color = float4(1.f, 1.f, 1.f, a);

  return vin;
}

struct GeometryOut
{
  float4 posHS : SV_POSITION;
  float2 texCoord : TEXCOORD;
  float4 color : COLOR;
};

/**
 * Expands particle from point to camera facing quads.
 * Calculates posHS and texCoord.
 */
[maxvertexcount(4)] void
DrawGS(point VertexOut gin[1], inout TriangleStream<GeometryOut> triStream)
{
  if (gin[0].flag == kUsing) {
    float3 look = normalize(gin[0].posWS - g_eyePosWS); /* ??? */
    float3 right = normalize(cross(float3(0, 1, 0), look));
    float3 up = cross(look, right);

    float halfHeight = 0.5f * gin[0].sizeWS.y;
    float halfWidth = 0.5f * gin[0].sizeWS.x;

    float4 p[4];
    p[0] = float4(gin[0].posWS - right * halfWidth + up * halfHeight, 1.f);
    p[1] = float4(gin[0].posWS + right * halfWidth + up * halfHeight, 1.f);
    p[2] = float4(gin[0].posWS - right * halfWidth - up * halfHeight, 1.f);
    p[3] = float4(gin[0].posWS + right * halfWidth - up * halfHeight, 1.f);

    GeometryOut gout;
    for (int i = 0; i < 4; i++) {
      gout.posHS = mul(p[i], g_viewProj);
      gout.texCoord = kQuadTexCoords[i];
      gout.color = gin[0].color;

      triStream.Append(gout);
    }
  }
}

float4
DrawPS(GeometryOut pin)
  : SV_TARGET
{
  float4 texColor = g_colorTex.Sample(kSamLinear, pin.texCoord);
  return pin.color * texColor;
}

technique11 DrawTech
{
  pass p0
  {
    SetVertexShader(CompileShader(vs_5_0, DrawVS()));
    SetHullShader(NULL);
    SetDomainShader(NULL);
    SetGeometryShader(CompileShader(gs_5_0, DrawGS()));
    SetPixelShader(CompileShader(ps_5_0, DrawPS()));

    SetBlendState(kAdditiveBlending, float4(0.0f, 0.0f, 0.0f, 0.0f),
                  0xffffffff);
    SetDepthStencilState(kNoDepthWrites, 0);
  }
}