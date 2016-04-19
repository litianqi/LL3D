#pragma once

#include "Math/Math.h"
#include "Core/Uncopyable.h"
#include "Component.h"
#include "Graphics/Base.h"

namespace LL3D
{
  class Transform;
}


namespace LL3D
{
  namespace Graphics
  {

    // Camera that can not be moved or rotated.
    class StaticCamera : public Component
    {
    public:
      StaticCamera(float fovY, float aspectRatio, float nearZ, float farZ);

      void setAspectRatio(float aspectRatio);
      // Updates cahed value (view matrix).
      void update() override;

      const Math::Matrix& projMaxtrix() const;

    private:
      float fovY_;
      float aspectRatio_;
      float nearZ_;
      float farZ_;

      // Cached values, updated one frame one time.
      Math::Matrix proj_;
    };


    class Camera : public StaticCamera, private Core::Uncopyable, private Base
    {
    public:
      Camera(const Transform& transform);

      const Math::Matrix& viewMatrix() const;
      const Math::Matrix& viewProjMatrix() const;
      DirectX::BoundingFrustum frustum() const;

      // Update cached values.
      void update() override;
      void writeToEffect() const;

    private:
      const Transform& transform_;

      // BEGIN Cached values, updated once per frame.
      DirectX::BoundingFrustum frustum_;
      Math::Matrix view_;
      Math::Matrix viewProj_;
      // END
    };

  }  // namespace Graphics
}  // namespace LL3D