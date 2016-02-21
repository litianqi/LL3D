#include "Material.fx"

struct AmbientLight {
  float4 light;
};

struct DirectionalLight {
  float4 light;
  float4 direction;
};

struct Attenuation {
  float a0;
  float a1;
  float a3;
};

struct PointLight {
  float4 light;
  float4 position;
  Attenuation attenuation;
  float range;
};

struct SpotLight {
  float4 light;
  float4 position;
  float4 direction;
  Attenuation attenuation;
  float range;
  float spot;
  float3 _pad;
};

float4 ComputeAmbientLight(Material mat,
  AmbientLight light) {
  return light.light * mat.ambient;
}

float4 ComputeDirectionalLight(Material mat, float4 normal,
  DirectionalLight light,
  float4 to_eye) {
  // Add oblique attenuation.
  float oblique = dot(-light.direction, normal);
  // Flatten to avoid dynamic branching.
  [flatten]
  if (oblique > 0.0f) {
    light.light *= oblique;

    // Compute diffuse reflection
    float4 diffuse = light.light * mat.diffuse;

    // Compute specular reflection
    float4 specular = light.light * mat.specular;

    // Add specular attenuation.
    float4 reflect_direction = reflect(-light.direction, normal);
    specular *= pow(max(dot(reflect_direction, to_eye), 0.0f), mat.spec_power);

    // Return
    return diffuse + specular;
  }
  else {
    return float4(0, 0, 0, 0);
  }

}

float4 ComputePointLight(Material mat, float4 pos, float4 normal,
  PointLight light,
  float4 to_eye) {
  return float4(0, 0, 0, 0);
}

float4 ComputeSpotLight(Material mat, float4 pos, float4 normal,
  SpotLight light,
  float4 to_eye) {
  return float4(0, 0, 0, 0);
}