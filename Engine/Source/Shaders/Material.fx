struct Material {
  float4 ambient;
  float4 diffuse;
  float4 specular;
  float4 emissive;
  float4 transparent;
  float specular_exponent;
  float shininess_strength;
  float opacity;
  float _pad;
};
