#include "Light.fx"

cbuffer PerFrame {
  AmbientLight g_ambient_light;
  DirectionalLight g_directional_light;
  PointLight g_point_light;
  SpotLight g_spot_light;
  float4 g_eye_pos_w;
};

cbuffer PerObject {
  float4x4 g_world;
  float4x4 g_wvp;
  Material g_material;
};

struct VertexIn {
  float4 pos_l  : POSITION;
  float4 normal_l : NORMAL;
};

struct VertexOut {
  float4 pos_h  : SV_POSITION;
  float4 pos_w  : POSITION;
  float4 normal_w : NORMAL;
};

VertexOut VS(VertexIn vin) {
  VertexOut vout;

  // Transform to homogeneous clip space.
  vout.pos_h = mul(vin.pos_l, g_wvp);

  // Transform to world space.
  vout.pos_w = mul(vin.pos_l, g_world);
  vout.normal_w = mul(vin.normal_l, g_world);

  return vout;
}

float4 PS(VertexOut pin) : SV_Target
{
  float4 to_eye = pin.pos_w - g_eye_pos_w;
  float4 a = ComputeAmbientLight(g_material, g_ambient_light);
  float4 d = ComputeDirectionalLight(g_material, pin.normal_w, g_directional_light, to_eye);
  float4 p = ComputePointLight(g_material, pin.pos_w, pin.normal_w, g_point_light, to_eye);
  float4 s = ComputeSpotLight(g_material, pin.pos_w, pin.normal_w, g_spot_light, to_eye);
  return a + d + p + s;
}

technique11 Tech {
  pass P0 {
    SetVertexShader(CompileShader(vs_5_0, VS()));
    SetGeometryShader(NULL);
    SetPixelShader(CompileShader(ps_5_0, PS()));
  }
}
