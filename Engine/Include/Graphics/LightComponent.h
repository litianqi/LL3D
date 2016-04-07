#pragma once

#include "Component.h"
#include "Math/Math.h"
#include "Graphics/Base.h"
#include "Graphics/LightFX.h"

namespace LL3D {
namespace Graphics {

class LightComponent : public Component, private Base {
public:
  enum LightType {Ambient, Directional, Point, Spot};

  LightComponent(LightType type);
  LightComponent(const AmbientLight& light);
  LightComponent(const DirectionalLight& light);
  LightComponent(const PointLight& light);
  LightComponent(const SpotLight& light);
  std::unique_ptr<Component> Clone() override;

  void Render() const;

  LightType GetLightType() const;

private:
  LightType type_;
  
  union Light
  {
    AmbientLight ambient;
    DirectionalLight directional;
    PointLight point;
    SpotLight spot;

    Light() noexcept;
    Light(const Light& rhs) noexcept;
    Light& operator=(const Light& rhs) noexcept;
    ~Light() noexcept;

  } light_;
};

}  // namespace Graphics
}  // namespace LL3D 