#include "Light.fx"
#include "Fog.fx"

cbuffer PerFrame {
  AmbientLightFX      g_ambient_light;
  DirectionalLightFX  g_directional_light;
  PointLightFX        g_point_light;
  SpotLightFX         g_spot_light;
  float4x4          g_viewProj;
  float4            g_eyePosWS;
  Fog               g_fog;
};

cbuffer PerObject {
  float4x4 g_world;
  float4x4 g_wvp;
  float4x4 g_texTransform;
  MaterialFX g_material;
};

Texture2D g_texture;
TextureCube g_cubeMap;

SamplerState g_sampler {
  Filter = ANISOTROPIC;
  MaxAnisotropy = 4;

  AddressU = WRAP;
  AddressV = WRAP;
};

//BlendState g_blend
//{
//    BlendEnable[0] = TRUE;
//    SrcBlend[0] = SRC_ALPHA;
//    DestBlend[0] = INV_SRC_ALPHA;
//    BlendOp[0] = ADD;
//    SrcBlendAlpha[0] = INV_DEST_ALPHA;
//    DestBlendAlpha[0] = ONE;
//    BlendOpAlpha[0] = ADD;
//    RenderTargetWriteMask[0] = 0x0F;
//};

struct VertexIn {
  float3 posLS  : POSITION;
  float3 normalLS : NORMAL;
  float2 texcoord : TEXCOORD;
  float3 tangent : TANGENT;
  float3 bitangent : BITANGENT;
};

struct VertexOut {
  float4 posPS  : SV_POSITION;
  float4 posWS  : POSITIONWS;
  float3 posLS  : POSITIONLS;
  float3 normal_w : NORMAL;
  float2 texcoord : TEXCOORD;
};

VertexOut VS(VertexIn vin) {
  VertexOut vout;
  
  vout.posLS = vin.posLS;
  vout.posWS = mul(float4(vin.posLS, 1.f), g_world);
  vout.normal_w = mul(float4(vin.normalLS, 0.f), g_world).xyz;
  vout.posPS = mul(vout.posWS, g_viewProj);
  vout.texcoord = /*mul(float4(*/vin.texcoord/*, 0.0, 1.0), g_texTransform).xy*/;

  return vout;
}

//[maxvertexcount(2)]
//void GS(point VertexOut gin[1], inout LineStream<VertexOut> gout)
//{
//  
//}

float4 PS(VertexOut pin, uniform  bool use_tex, uniform bool use_alpha_clip) : SV_Target
{
  float4 result;
  
  result.a = g_material.opacity;

  // Alpha clip
  if (use_alpha_clip) {
    clip(result.a - 0.1f);
  }

  // Shadow do not need texturing and lighting.
  if (g_material.is_shadow) {  
    result.xyz = g_material.diffuse;
    return result;
  }

  // Texturing
  float3 texDiffuse;
  if (use_tex) {
    float3 cubeMapDiffuse = g_cubeMap.Sample(g_sampler, pin.posLS).xyz;
    float3 otherDiffuse = g_texture.Sample(g_sampler, pin.texcoord).xyz;
      
    texDiffuse = cubeMapDiffuse + otherDiffuse;
  }

  // Lighting
  float3 viewDir = g_eyePosWS.xyz - pin.posWS.xyz;
  float3 ambient_contribution = ApplyAmbientLight(g_material, g_ambient_light);
  float3 directional_contribution = ApplyDirectionalLight(g_material, 
    g_directional_light, pin.normal_w, viewDir);
  float3 point_contribution = ApplyPointLight(g_material,
    g_point_light, pin.posWS.xyz, pin.normal_w, viewDir);
  float3 spot_contribution = ApplySpotLight(g_material,
    g_spot_light, pin.posWS.xyz, pin.normal_w, viewDir);
  
  float3 lightColor = ambient_contribution + directional_contribution +
    point_contribution + spot_contribution;

  // TODO: better way?
  result.xyz = max(lightColor, float3(0, 0, 0)) * 0.4f + texDiffuse * 0.6f;

  return result;

  //// Fogging
  ///*float distance_to_eye = length(to_eye);
  //float s = saturate((distance_to_eye - g_fog.start) / g_fog.range);
  //lit_color = (1 - s) * lit_color + s * g_fog.color;*/
}

technique11 Tech {
  pass P0 {
    SetVertexShader(CompileShader(vs_5_0, VS()));
    SetGeometryShader(NULL);
    SetPixelShader(CompileShader(ps_5_0, PS(true, false)));
  }
}
