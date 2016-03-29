#pragma once

#include <list>
#include <DirectXMath.h>
#include "../Math/Math.h"
#include "../Component.h"
#include "Base.h"

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

  Camera(Frustum frustum, DirectX::FXMVECTOR forward_vector);
  std::unique_ptr<Component> Clone() override;

  void SetFrustum(const Frustum& frustum);
  void SetPosition(DirectX::FXMVECTOR p);
  void SetForwardVector(DirectX::FXMVECTOR v);

  Math::Matrix GetViewMatrix() const;
  Math::Matrix GetViewProjectionMatrix() const;
  const Frustum& GetFrustum() const;
  DirectX::XMVECTOR GetPosition() const;
  DirectX::XMVECTOR GetForwardVector() const;
  DirectX::XMVECTOR GetRightVector() const;
  DirectX::XMVECTOR GetUpVector() const;

  //>
  // Writes properties to shader buffer.
  //
  void Update() override;

  DirectX::XMVECTOR ViewToWorldPosition(DirectX::FXMVECTOR p) const;
  DirectX::XMVECTOR WorldToViewPosition(DirectX::FXMVECTOR p) const;

protected:
  // Came coordinate system with coordinate relative to world space.
  DirectX::XMVECTOR forward_vector_;

private:
  Frustum frustum_;
};

}  // namespace Graphics
}  // namespace LL3D
