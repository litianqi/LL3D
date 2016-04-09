#pragma once

#include <list>
#include "Math/Vector.h"
#include "Math/Matrix.h"
#include "Component.h"
#include "Graphics/Base.h"

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

    void SetAspectRatio(float aspect_ratio);

    Math::Matrix GetProjectionMaxtrix() const;

  private:
    float radian_fov_y_;
    float aspect_ratio_;
    float z_near_;
    float z_far_;
  };

  Camera(Frustum frustum, Math::Vector3 forward_vector);
  std::unique_ptr<Component> Clone() override;

  void SetFrustum(const Frustum& frustum);
  void SetPosition(Math::Vector3 p);
  void SetForwardVector(Math::Vector3 v);

  Math::Matrix GetViewMatrix() const;
  Math::Matrix GetViewProjectionMatrix() const;
  const Frustum& GetFrustum() const;
  Math::Vector3 GetPosition() const;
  Math::Vector3 GetForwardVector() const;
  Math::Vector3 GetRightVector() const;
  Math::Vector3 GetUpVector() const;

  //>
  // Writes properties to shader buffer.
  //
  void Render() const;

  Math::Vector3 ViewToWorldPosition(Math::Vector3 p) const;
  Math::Vector3 WorldToViewPosition(Math::Vector3 p) const;

protected:
  // Came coordinate system with coordinate relative to world space.
  Math::Vector3 forward_vector_;

private:
  Frustum frustum_;
};

}  // namespace Graphics
}  // namespace LL3D
