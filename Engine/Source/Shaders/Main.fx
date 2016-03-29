#include "Light.fx"
#include "Fog.fx"

cbuffer PerFrame {
  AmbientLightFX      g_ambient_light;
  DirectionalLightFX  g_directional_light;
  PointLightFX        g_point_light;
  SpotLightFX         g_spot_light;
  float4x4          g_view;
  float4x4          g_projection;
  float4            g_eye_pos_w;
  Fog               g_fog;
};

cbuffer PerObject {
  float4x4 g_world;
  float4x4 g_wvp;
  float4x4 g_texture_transform;
  MaterialFX g_material;
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
  float2 texcoord : TEXCOORD;
  float3 tangent : TANGENT;
  float3 bitangent : BITANGENT;
};

struct VertexOut {
  float4 pos_h  : SV_POSITION;
  float4 pos_w  : POSITION;
  float3 normal_w : NORMAL;
  float2 texcoord : TEXCOORD;
};

VertexOut VS(VertexIn vin) {
  VertexOut vout;

  // Transform to world space.
  vout.pos_w = mul(float4(vin.pos_l, 1.f), g_world);
  vout.normal_w = mul(float4(vin.normal_l, 0.f), g_world).xyz;

  // Transform to homogeneous clip space.
  vout.pos_h = mul(vout.pos_w, g_view);
  vout.pos_h = mul(vout.pos_h, g_projection);

  vout.texcoord = /*mul(float4(*/vin.texcoord/*, 0.0, 1.0), g_texture_transform).xy*/;

  return vout;
}

float4 PS(VertexOut pin, uniform  bool use_tex, uniform bool use_alpha_clip) : SV_Target
{
  float4 result;
  
  result.a = g_material.opacity;

  // Alpha clip
  if (use_alpha_clip) {
    clip(result.a - 0.1f);
  }

  // Texturing
  float3 tex_diffuse;
  if (use_tex) {
    tex_diffuse = g_texture.Sample(g_sampler, pin.texcoord).xyz;
  }

  // Lighting
  float3 view_dir = g_eye_pos_w.xyz - pin.pos_w.xyz;
  float3 ambient_contribution = ApplyAmbientLight(g_material, g_ambient_light);
  float3 directional_contribution = ApplyDirectionalLight(g_material, 
    g_directional_light, pin.normal_w, view_dir);
  float3 point_contribution = ApplyPointLight(g_material,
    g_point_light, pin.pos_w.xyz, pin.normal_w, view_dir);
  float3 spot_contribution = ApplySpotLight(g_material,
    g_spot_light, pin.pos_w.xyz, pin.normal_w, view_dir);
  
  result.xyz = ambient_contribution + directional_contribution +\
    point_contribution + spot_contribution;

  // TODO: better way?
  result.xyz = (result.xyz + tex_diffuse) / 2.f;

  return result;

  // Lighting
  /*float4 to_eye = pin.pos_w - g_eye_pos_w;
  float3 ambinet = ComputeAmbientLight(g_material, g_ambient_light);
  float3 directiona*/
  /*float4 direct = ComputeDirectionalLight(g_material, pin.normal_w, g_directional_light, to_eye);
  float4 poin = ComputePointLight(g_material, pin.pos_w, pin.normal_w, g_point_light, to_eye);
  float4 spot = ComputeSpotLight(g_material, pin.pos_w, pin.normal_w, g_spot_light, to_eye);*/
  //float3 light_color = ambinet /*+ direct + poin + spot*/;

  //float4 lit_color;
  //float4 texture_color = float4(1, 1, 1, 1);
  //if (use_tex) {
  //  texture_color = g_texture.Sample(g_sampler, pin.texcoord);
  //  if (use_alpha_clip) {
  //    clip(texture_color.a - 0.1f);
  //  }
  //  lit_color.xyz = texture_color * light_color;  // TODO: modulate with late add.
  //}
  //else {
  //  lit_color.xyz = light_color;
  //}

  //// Fogging
  ///*float distance_to_eye = length(to_eye);
  //float s = saturate((distance_to_eye - g_fog.start) / g_fog.range);
  //lit_color = (1 - s) * lit_color + s * g_fog.color;*/

  //// Common to take alpha from diffuse material and texture.
  //lit_color.a = g_material.opacity;

  //return lit_color;
}

technique11 Tech {
  pass P0 {
    SetBlendState(g_blend, float4(0, 0, 0, 0), 0xffffffff);
    SetVertexShader(CompileShader(vs_5_0, VS()));
    SetGeometryShader(NULL);
    SetPixelShader(CompileShader(ps_5_0, PS(true, false)));
  }
}
