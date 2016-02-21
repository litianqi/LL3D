#pragma once

#include <DirectXMath.h>

namespace LL3D {

using namespace DirectX;

struct AmbientLight {
  XMVECTOR light;
};

struct DirectionalLight {
  XMVECTOR light;
  XMVECTOR direction;
};

struct Attenuation {
  float a0;
  float a1;
  float a3;
};

struct PointLight {
  XMVECTOR light;
  XMVECTOR position;
  Attenuation attenuation;
  float range;
};

struct SpotLight {
  XMVECTOR light;
  XMVECTOR position;
  XMVECTOR direction;
  Attenuation attenuation;
  float range;
  float spot_power;
  DirectX::XMFLOAT3 _pad;  // Pad the last lfoat so we can set an array of SpotLights if we wanted.
};

// TODO: light vector can only be set 0/1 element.
struct Lights {
  std::vector<AmbientLight> ambients;
  std::vector<DirectionalLight> directionals;
  std::vector<PointLight> points;
  std::vector<SpotLight> spots;
};

}  // namespace LL3D 

