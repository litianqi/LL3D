#include "Material.fx"

struct AmbientLight {
  float4 color;
};

struct DirectionalLight {
  float4 color;
  float4 direction;
};

struct Attenuation {
  float a0;
  float a1;
  float a2;
};

struct PointLight {
  float4 color;
  float4 position;
  Attenuation distance_att;
  float range;
};

struct SpotLight {
  float4 color;
  float4 position;
  float4 direction;
  Attenuation distance_att;
  float range;
  float cone_att;
  float3 _pad;
};

float4 ComputeAmbientLight(Material mat,
  AmbientLight light) {
  return light.color * mat.diffuse;
}

float4 ComputeDirectionalLight(Material mat, float4 normal, DirectionalLight light, float4 to_eye) {
  
  // Compute oblique attenuation.
  light.direction = normalize(light.direction);
  float oblique_att = dot(-light.direction, normal);

  // Flatten to avoid dynamic branching.
  [flatten]
  if (oblique_att > 0.0f) {
    light.color *= oblique_att;

    // Compute diffuse reflection
    float4 diffuse = light.color * mat.diffuse;

    // Compute specular reflection:

    // Compute specular cone attenuation.
    float4 reflect_direction = reflect(-light.direction, normal);
    reflect_direction = normalize(reflect_direction);
    to_eye = normalize(to_eye);
    float spec_cone_att = pow(max(dot(reflect_direction, to_eye), 0.0f), mat.specular_exponent);
   
    float4 specular = light.color * mat.specular * spec_cone_att; 

    return diffuse + specular;
  }
  else {
    return float4(0, 0, 0, 0);
  }
}

float4 ComputePointLight(Material mat, float4 pos, float4 normal,
  PointLight light,
  float4 to_eye) {
  // Test range.
  float4 to_pixel = pos - light.position;
  float distance = length(to_pixel);
  if (distance > light.range)
    return float4(0, 0, 0, 0);

  // Add distance attenuation.
  float distance_att = 1.0f / dot(float3(light.distance_att),
    float3(1, distance, distance * distance));
  light.color *= distance_att;

  DirectionalLight final_light;
  final_light.color = light.color;
  final_light.direction = to_pixel;
  return ComputeDirectionalLight(mat, normal, final_light, to_eye);
}

float4 ComputeSpotLight(Material mat, float4 pos, float4 normal,
  SpotLight light,
  float4 to_eye) {
  // Test range.
  float4 to_pixel = pos - light.position;
  float distance = length(to_pixel);
  if (distance > light.range)
    return float4(0, 0, 0, 0);

  // Add distance attenuation.
  float distance_att = 1.0f / dot(float3(light.distance_att),
    float3(1, distance, distance * distance));
  light.color *= distance_att;

  // Add spot cone attenuation.
  to_pixel = normalize(to_pixel);
  light.direction = normalize(light.direction);
  float spot_cone_att = pow(max(dot(to_pixel, light.direction), 0), light.cone_att);
  light.color *= spot_cone_att;

  DirectionalLight final_light;
  final_light.color = light.color;
  final_light.direction = to_pixel;
  return ComputeDirectionalLight(mat, normal, final_light, to_eye);
}