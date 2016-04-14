#pragma once

#include "Component.h"
#include "Math/Math.h"
#include "Graphics/Base.h"
#include "Graphics/LightFX.h"

namespace LL3D {
class Transform;
}

namespace LL3D {
namespace Graphics {

class LightComponent : public Component, private Base {
public:
  enum LightType {Ambient, Directional, Point, Spot};

  LightComponent(const Transform& transform, LightType type);
  LightComponent(const Transform& transform, const AmbientLight& light);
  LightComponent(const Transform& transform, const DirectionalLight& light);
  LightComponent(const Transform& transform, const PointLight& light);
  LightComponent(const Transform& transform, const SpotLight& light);

  void writeToEffect() const;

  LightType lightType() const;

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
  const Transform& transform_;
};

}  // namespace Graphics
}  // namespace LL3D 