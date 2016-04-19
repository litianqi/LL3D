#pragma once

#include "Math/Math.h"
#include "Component.h"
#include "Graphics/Base.h"

namespace LL3D
{
  namespace Graphics
  {

    // Fog used in effect files.
    struct EffectFog
    {
      // Color of fog. Note: Alpha of color is unused.
      Math::Color color;
      float start;
      float range;
      Math::Vector2 __pad;
    };

    class Fog : public Component, private Graphics::Base
    {
    public:

      // Note: Alpha of color is unused.
      Fog(Math::Color color, float start, float range);
      void update() override;

    private:

      EffectFog data_;

    };

  }  // namespace Graphics
}  // namespace LL3D