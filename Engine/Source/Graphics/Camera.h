#pragma once

#include <list>
#include "Math/Math.h"
#include "Component.h"
#include "Graphics/Base.h"

namespace LL3D {

class Transform;

}


namespace LL3D {
namespace Graphics {

// Non-rollable Camera. 
// Non-rollable means Up and Forward vector always on a YZ panel(of world space) 
// roated some degree around Y.
class Camera : public Component, private Base {
public:

  // Frustum properties of a Camera.
  class Frustum {
  public:

    Frustum(float radian_fov_y, float aspect_ratio, float z_near, float z_far);
    void setAspectRatio(float aspect_ratio);
    Math::Matrix projectionMaxtrix() const;

  private:

    float radian_fov_y_;
    float aspect_ratio_;
    float z_near_;
    float z_far_;

  };


  Camera(Transform& transform, Frustum frustum, Math::Vector3 forward_vector);
 
  // Writes properties to shader buffer.
  void render() const;

  void SetFrustum(const Frustum& frustum);
  void SetPosition(Math::Vector3 p);
  void SetForwardVector(Math::Vector3 v);

  Math::Matrix viewMatrix() const;
  Math::Matrix viewProjectionMatrix() const;
  const Frustum& frustum() const;
  Math::Vector3 GetPosition() const;
  Math::Vector3 GetForwardVector() const;
  Math::Vector3 GetRightVector() const;
  Math::Vector3 GetUpVector() const;
  DirectX::BoundingFrustum boundingFrustum() const;

  Math::Vector3 convertViewToWorld(Math::Vector3 position) const;
  Math::Vector3 convertWorldToView(Math::Vector3 position) const;

protected:

  // Came coordinate system with coordinate relative to world space.
  Math::Vector3 forward_vector_;

private:

  Frustum frustum_;
  Transform& transform_;

};

}  // namespace Graphics
}  // namespace LL3D
