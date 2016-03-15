#pragma once

#include <vector>
#include <DirectXMath.h>
#include "../Component.h"
#include "Base.h"

namespace LL3D {
namespace Graphics {

struct AmbientLight : public Component, private Base {
  struct Data {
    DirectX::XMVECTOR color;
  } data;

  AmbientLight(const Data& d);
  std::unique_ptr<Component> Clone() override;
  
  void Update() override;
};

// TODO: add constuctor. (virtual class required to have a constructor)

struct DirectionalLight : public Component, private Base {
  DirectX::XMVECTOR color;
  DirectX::XMVECTOR direction;

  std::unique_ptr<Component> Clone() override;
  void Update() override;
};

// Attention! Couldn't all a0, a1, a2 be zero, the result light is unpredictable.
struct Attenuation {
  float a0;
  float a1;
  float a2;
};

struct PointLight : public Component, private Base {
  DirectX::XMVECTOR color;
  DirectX::XMVECTOR position;
  Attenuation distance_att;
  float range;

  std::unique_ptr<Component> Clone() override;
  void Update() override;
};

struct SpotLight : public Component, private Base {
  DirectX::XMVECTOR color;
  DirectX::XMVECTOR position;
  DirectX::XMVECTOR direction;
  Attenuation distance_att;
  float range;
  float cone_att;
  DirectX::XMFLOAT3 _pad;  // Pad the last lfoat so we can set an array of SpotLights if we wanted.

  std::unique_ptr<Component> Clone() override;
  void Update() override;
};

}  // namespace Graphics
}  // namespace LL3D 

