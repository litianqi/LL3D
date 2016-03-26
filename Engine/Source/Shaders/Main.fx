#include "Light.fx"
#include "Fog.fx"

cbuffer PerFrame {
  AmbientLight      g_ambient_light;
  DirectionalLight  g_directional_light;
  PointLight        g_point_light;
  SpotLight         g_spot_light;
  float4x4          g_view;
  float4x4          g_projection;
  float4            g_eye_pos_w;
  Fog               g_fog;
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

BlendState g_blend
{
    BlendEnable[0] = TRUE;
    SrcBlend[0] = SRC_ALPHA;
    DestBlend[0] = INV_SRC_ALPHA;
    BlendOp[0] = ADD;
    SrcBlendAlpha[0] = INV_DEST_ALPHA;
    DestBlendAlpha[0] = ONE;
    BlendOpAlpha[0] = ADD;
    RenderTargetWriteMask[0] = 0x0F;
};

struct VertexIn {
  float3 pos_l  : POSITION;
  float3 normal_l : NORMAL;
  float2 texture_coordinate : TEXCOORD;
  float3 tangent : TANGENT;
  float3 bitangent : BITANGENT;
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
  vout.pos_h = mul(float4(vin.pos_l, 1.f), mul(g_world, mul(g_view, g_projection)));

  // Transform to world space.
  vout.pos_w = mul(float4(vin.pos_l, 1.f), g_world);
  vout.normal_w = mul(float4(vin.normal_l, 0.f), g_world);

  vout.texture_coordinate = mul(float4(vin.texture_coordinate, 0.0, 1.0), g_texture_transform).xy;

  return vout;
}

float4 PS(VertexOut pin, uniform  bool use_tex, uniform bool use_alpha_clip) : SV_Target
{
  // Texturing



  // Lighting
  float4 to_eye = pin.pos_w - g_eye_pos_w;
  float4 ambinet = ComputeAmbientLight(g_material, g_ambient_light);
  float4 direct = ComputeDirectionalLight(g_material, pin.normal_w, g_directional_light, to_eye);
  float4 poin = ComputePointLight(g_material, pin.pos_w, pin.normal_w, g_point_light, to_eye);
  float4 spot = ComputeSpotLight(g_material, pin.pos_w, pin.normal_w, g_spot_light, to_eye);
  float4 light_color = ambinet + direct + poin + spot;

  float4 lit_color;
  float4 texture_color = float4(1, 1, 1, 1);
  if (use_tex) {
    texture_color = g_texture.Sample(g_sampler, pin.texture_coordinate);
    if (use_alpha_clip) {
      clip(texture_color.a - 0.1f);
    }
    lit_color = texture_color * light_color;  // TODO: modulate with late add.
  }
  else {
    lit_color = light_color;
  }

  // Fogging
  /*float distance_to_eye = length(to_eye);
  float s = saturate((distance_to_eye - g_fog.start) / g_fog.range);
  lit_color = (1 - s) * lit_color + s * g_fog.color;*/

  // Common to take alpha from diffuse material and texture.
  lit_color.a = texture_color.a * g_material.diffuse.a;

  return lit_color;
}

technique11 Tech {
  pass P0 {
    SetBlendState(g_blend, float4(0, 0, 0, 0), 0xffffffff);
    SetVertexShader(CompileShader(vs_5_0, VS()));
    SetGeometryShader(NULL);
    SetPixelShader(CompileShader(ps_5_0, PS(true, true)));
  }
}
