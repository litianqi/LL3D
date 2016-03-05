#include "Light.fx"

cbuffer PerFrame {
  AmbientLight g_ambient_light;
  DirectionalLight g_directional_light;
  PointLight g_point_light;
  SpotLight g_spot_light;
  float4x4 g_view;
  float4x4 g_projection;
  float4 g_eye_pos_w;
};

cbuffer PerObject {
  float4x4 g_world;
  float4x4 g_wvp;
  float4x4 g_texture_transform;
  Material g_material;
};

Texture2D g_texture;

SamplerState g_sampler {
  Filter = ANISOTROPIC;
  MaxAnisotropy = 4;

  AddressU = WRAP;
  AddressV = WRAP;
};

struct VertexIn {
  float4 pos_l  : POSITION;
  float4 normal_l : NORMAL;
  float2 texture_coordinate : TEXCOORD;
};

struct VertexOut {
  float4 pos_h  : SV_POSITION;
  float4 pos_w  : POSITION;
  float4 normal_w : NORMAL;
  float2 texture_coordinate : TEXCOORD;
};

VertexOut VS(VertexIn vin) {
  VertexOut vout;

  // Transform to homogeneous clip space.
  vout.pos_h = mul(vin.pos_l, mul(g_world, mul(g_view, g_projection)));

  // Transform to world space.
  vout.pos_w = mul(vin.pos_l, g_world);
  vout.normal_w = mul(vin.normal_l, g_world);

  vout.texture_coordinate = mul(float4(vin.texture_coordinate, 0.0, 1.0), g_texture_transform).xy;

  return vout;
}

float4 PS(VertexOut pin, uniform  bool use_tex) : SV_Target
{
  // Calculate light color.
  float4 to_eye = pin.pos_w - g_eye_pos_w;
  float4 a = ComputeAmbientLight(g_material, g_ambient_light);
  float4 d = ComputeDirectionalLight(g_material, pin.normal_w, g_directional_light, to_eye);
  float4 p = ComputePointLight(g_material, pin.pos_w, pin.normal_w, g_point_light, to_eye);
  float4 s = ComputeSpotLight(g_material, pin.pos_w, pin.normal_w, g_spot_light, to_eye);
  float4 light_color = a + d + p + s;

  float4 lit_color;
  if (use_tex) {
    float4 texture_color = g_texture.Sample(g_sampler, pin.texture_coordinate);
    lit_color = texture_color * light_color;  // TODO: modulate with late add.
    // Common to take alpha from diffuse material and texture.
    lit_color.a = texture_color.a * g_material.diffuse.a;
  }
  else {
    lit_color = light_color;
  }

  return lit_color;
}

technique11 Tech {
  pass P0 {
    SetVertexShader(CompileShader(vs_5_0, VS()));
    SetGeometryShader(NULL);
    SetPixelShader(CompileShader(ps_5_0, PS(true)));
  }
}
