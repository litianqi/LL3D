#include "Particle.fx"

cbuffer __constants__
{
  uint kNone = 0;
  uint kUsing = 1;
  float3 kAccelWS = { 0.0f, -7.8f, 0.0f };
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
[maxvertexcount(51)] void
StreamOutGS(point ParticleVertex gin[1],
            inout PointStream<ParticleVertex> pointStream)
{
  gin[0].age += g_timeStep;

  if (gin[0].flag == kNone) {
    for (int i = 0; i < 50; i++) {
      ParticleVertex gout;

      float3 relatePos = 200.f * randf3(i / 50.f);
      relatePos.y = 200.f;
      gout.posWS = g_emitPosWS + relatePos;

      float3 velWS = randf3(0.f);
      velWS.y = -100.f;
      gout.velWS = velWS;

      gout.age = 0;
      gout.flag = kUsing;
      gout.sizeWS = float2(0, 0); /* We do not use this property. */

      pointStream.Append(gout);
    }

    pointStream.Append(gin[0]);
  } else {
    if (gin[0].age < 3.5f) {
      pointStream.Append(gin[0]);
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
  uint flag : FLAG;
};

/**
* Calculates position.
*/
VertexOut
DrawVS(ParticleVertex vin)
{
  VertexOut vout;

  float t = vin.age;
  vout.posWS = 0.5f * kAccelWS * t * t + vin.velWS * t + vin.posWS;

  vout.flag = vin.flag;

  return vout;
}

struct GeometryOut
{
  float4 posHS : SV_POSITION;
  float2 texCoord : TEXCOORD;
};

/**
* Expands particle from point to line.
* Calculates posHS and texCoord.
*/
[maxvertexcount(2)] void
DrawGS(point VertexOut gin[1], inout LineStream<GeometryOut> lineStream)
{
  if (gin[0].flag == kUsing) {
    GeometryOut gout0;
    gout0.posHS = mul(float4(gin[0].posWS + 0.7f * kAccelWS, 1.f), g_viewProj);
    gout0.texCoord = float2(0, 0);
    lineStream.Append(gout0);

    GeometryOut gout1;
    gout1.posHS = mul(float4(gin[0].posWS, 1.f), g_viewProj);
    gout1.texCoord = float2(1, 1);
    lineStream.Append(gout1);
  }
}

float4
DrawPS(GeometryOut pin)
  : SV_TARGET
{
  return g_colorTex.Sample(kSamLinear, pin.texCoord);
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