#include "../Graphics/MaterialFX.h"
#include "../Graphics/LightFX.h"

float3
ApplyDiffuseAlbedo(float3 diffuseAlbedo, float3 diffuseLight)
{
  return diffuseAlbedo * diffuseLight;
}

float3
ApplySpecularAlbedoAndCone(float3 specularAlbedo, float shinness,
                           float3 specularLight, float3 lightDir, float3 normal,
                           float3 viewDir)
{
  float3 reflectDir = reflect(lightDir, normal);
  reflectDir = normalize(reflectDir);

  float specularFactor = pow(max(dot(reflectDir, viewDir), 0.0f), shinness);

  return (specularLight * specularAlbedo) * specularFactor;
}

void
ApplyDistanceAttenuation(inout float3 diffuseLight, inout float3 specularLight,
                         float3 attenuation, float distance)
{
  // Add distance attenuation.
  float att_factor =
    1.f / dot(attenuation, float3(1.f, distance, distance * distance));

  diffuseLight *= att_factor;
  specularLight *= att_factor;
}

//>
// Returns what color will reflect out after a light hits a point.
// lightDiffuse/lightSpecular reflects what color light are when lgiht hits
// point. So attenuation etc. should be pre-computed.
//
float3 LightHitPoint(MaterialFX mat, float3 lightDiffuse, float3 lightSpecular,
                     float3 lightDir, // World-space vector from light to point
                     float3 pointNormal, // World-space normal
                     float3 viewDir)     // World-space vector from eye to point
{
  // Normalize vectors
  lightDir = normalize(lightDir);
  pointNormal = normalize(pointNormal);
  viewDir = normalize(viewDir);

  // Light oblique
  float obliqueFactor = dot(pointNormal, -lightDir);
  if (obliqueFactor <= 0)
    return 0;

  lightDiffuse *= obliqueFactor;
  lightSpecular *= obliqueFactor;

  // Diffuse albedo.
  float3 diffuse = ApplyDiffuseAlbedo(mat.diffuse, lightDiffuse);

  // Specular albedo and specular cone.
  float3 specular = ApplySpecularAlbedoAndCone(
    mat.specular, mat.shininess, lightSpecular, lightDir, pointNormal, viewDir);

  return diffuse + specular;
}

float3
ApplyAmbientLight(MaterialFX mat, AmbientLightFX light)
{
  // TODO: better way?

  return mat.ambient * light.ambient;
}

float3 ApplyDirectionalLight(
  MaterialFX mat, DirectionalLightFX light,
  float3 pointNormal, // World-space normal
  float3 viewDir)     // World-space vector from eye to point
{
  // Directional directly hits point, no attenuation etc.
  return LightHitPoint(mat, light.diffuse, light.specular, light.direction,
                       pointNormal, viewDir);
}

float3 ApplyPointLight(MaterialFX mat, PointLightFX light,
                       float3 pointPos,    // World-space point position
                       float3 pointNormal, // World-space normal
                       float3 viewDir) // World-space vector from eye to point
{
  float distance = length(light.position - pointPos);
  ApplyDistanceAttenuation(light.diffuse, light.specular, light.attenuation,
                           distance);

  float3 lightDir = pointPos - light.position;
  return LightHitPoint(mat, light.diffuse, light.specular, lightDir,
                       pointNormal, viewDir);
}

float3 ApplySpotLight(MaterialFX mat, SpotLightFX light,
                      float3 pointPos,    // World-space point position
                      float3 pointNormal, // World-space normal
                      float3 viewDir) // World-space vector from eye to point
{
  // Normalize vectors
  light.direction = normalize(light.direction);
  pointNormal = normalize(pointNormal);
  viewDir = normalize(viewDir);

  float distance = length(light.position - pointPos);
  ApplyDistanceAttenuation(light.diffuse, light.specular, light.attenuation,
                           distance);

  float3 light2point = pointPos - light.position;
  // TODO: inner_cone_angle, outer_cone_angle..
  float spot_factor =
    pow(max(dot(light2point, light.direction), 0), light.innerConeAngle);
  light.diffuse *= spot_factor;
  light.specular *= spot_factor;

  return LightHitPoint(mat, light.diffuse, light.specular, light.direction,
                       pointNormal, viewDir);
}