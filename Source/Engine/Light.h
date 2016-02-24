#pragma once

#include <DirectXMath.h>

namespace LL3D {

using namespace DirectX;

struct AmbientLight {
  XMVECTOR color;
};

struct DirectionalLight {
  XMVECTOR color;
  XMVECTOR direction;
};

// Attention! Couldn't all a0, a1, a2 be zero, the result light is unpredictable.
struct Attenuation {
  float a0;
  float a1;
  float a2;
};

struct PointLight {
  XMVECTOR color;
  XMVECTOR position;
  Attenuation distance_att;
  float range;
};

struct SpotLight {
  XMVECTOR color;
  XMVECTOR position;
  XMVECTOR direction;
  Attenuation distance_att;
  float range;
  float cone_att;
  XMFLOAT3 _pad;  // Pad the last lfoat so we can set an array of SpotLights if we wanted.
};

// TODO: light vector can only be set 0/1 element.
struct Lights {
  std::vector<AmbientLight> ambients;
  std::vector<DirectionalLight> directionals;
  std::vector<PointLight> points;
  std::vector<SpotLight> spots;
};

}  // namespace LL3D 

