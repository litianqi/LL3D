#include "../Graphics/MaterialFX.h"
#include "../Graphics/LightFX.h"

float3 ApplyDiffuseAlbedo(float3 diffuse_albedo, float3 diffuse_light)
{
  return diffuse_albedo * diffuse_light;
}

float3 ApplySpecularAlbedoAndCone(
  float3 specular_albedo, 
  float shinness,
  float3 specular_light,
  float3 light_dir,
  float3 normal,
  float3 view_dir)
{
  float3 reflect_dir = reflect(light_dir, normal);
  reflect_dir = normalize(reflect_dir);
  
  float specular_factor = pow(max(dot(reflect_dir, view_dir), 0.0f), shinness);

  return (specular_light * specular_albedo) * specular_factor;
}

void ApplyDistanceAttenuation(
  inout float3 diffuse_light,
  inout float3 specular_light,
  float3 attenuation,
  float distance
  )
{
  // Add distance attenuation.
  float att_factor = 1.f / 
    dot(attenuation, float3(1.f, distance, distance * distance));
  
  diffuse_light *= att_factor;
  specular_light *= att_factor;
}

//>
// Returns what color will reflect out after a light hits a point.
// light_diffuse/light_specular reflects what color light are when lgiht hits 
// point. So attenuation etc. should be pre-computed.
// 
float3 LightHitPoint(
  MaterialFX mat,
  float3 light_diffuse,
  float3 light_specular,
  float3 light_dir,  // World-space vector from light to point
  float3 point_normal,  // World-space normal
  float3 view_dir)  // World-space vector from eye to point
{
  // Normalize vectors
  light_dir = normalize(light_dir);
  point_normal = normalize(point_normal);
  view_dir = normalize(view_dir);

  // Light oblique
  float oblique_factor = dot(point_normal, -light_dir);
  if (oblique_factor <= 0)
    return 0;
  
  light_diffuse *= oblique_factor;
  light_specular *= oblique_factor;

  // Diffuse albedo.
  float3 diffuse = ApplyDiffuseAlbedo(mat.diffuse, light_diffuse);
  
  // Specular albedo and specular cone.
  float3 specular = ApplySpecularAlbedoAndCone(mat.specular, mat.shininess, light_specular,
    light_dir, point_normal, view_dir);
  
  return diffuse + specular;
}

float3 ApplyAmbientLight(
  MaterialFX mat,
  AmbientLightFX light
  )
{
  // TODO: better way?

  return mat.ambient * light.ambient;
}

float3 ApplyDirectionalLight(
  MaterialFX mat,
  DirectionalLightFX light, 
  float3 point_normal,  // World-space normal
  float3 view_dir)  // World-space vector from eye to point
{
  // Directional directly hits point, no attenuation etc.
  return LightHitPoint(mat, light.diffuse, light.specular, light.direction,
    point_normal, view_dir);
}

float3 ApplyPointLight(
  MaterialFX mat,
  PointLightFX light,
  float3 point_pos,  // World-space point position
  float3 point_normal,  // World-space normal
  float3 view_dir)  // World-space vector from eye to point
{
  float distance = length(light.position - point_pos);
  ApplyDistanceAttenuation(light.diffuse, light.specular, light.attenuation, distance);

  float3 light_dir = point_pos - light.position;
  return LightHitPoint(mat, light.diffuse, light.specular, light_dir,
    point_normal, view_dir);
}

float3 ApplySpotLight(
  MaterialFX mat,
  SpotLightFX light,
  float3 point_pos,  // World-space point position
  float3 point_normal,  // World-space normal
  float3 view_dir)  // World-space vector from eye to point
{
  // Normalize vectors
  light.direction = normalize(light.direction);
  point_normal = normalize(point_normal);
  view_dir = normalize(view_dir);

  float distance = length(light.position - point_pos);
  ApplyDistanceAttenuation(light.diffuse, light.specular, light.attenuation, distance);

  float3 light2point = point_pos - light.position;
  // TODO: inner_cone_angle, outer_cone_angle..
  float spot_factor = pow(max(dot(light2point, light.direction), 0), light.inner_cone_angle);
  light.diffuse *= spot_factor;
  light.specular *= spot_factor;

  return LightHitPoint(mat, light.diffuse, light.specular, light.direction,
    point_normal, view_dir);
}