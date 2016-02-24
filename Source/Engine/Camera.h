#pragma once

#include <DirectXMath.h>

namespace LL3D {

using namespace DirectX;

// Non-rollable Camera. 
// Non-rollable means Up and Forward vector always on a YZ panel(of world space) 
// roated some degree around Y.

class Camera {
public:
  
  // Frustum properties of a Camera.
  class Frustum {
  public:
    Frustum(float radian_fov_y, float aspect_ratio, float z_near, float z_far);
    
    void SetAspectRatio(float aspect_ratio);

    XMMATRIX GetProjectionMaxtrix() const;
  
  private:
    float radian_fov_y_;
    float aspect_ratio_;
    float z_near_;
    float z_far_;
  };

  Camera(Frustum frustum, FXMVECTOR position, FXMVECTOR forward_vector);
  
  void SetFrustum(const Frustum& frustum);
  void SetPosition(FXMVECTOR p);
  void SetForwardVector(FXMVECTOR v);
 
  XMMATRIX GetViewMatrix() const;
  XMMATRIX GetViewProjectionMatrix() const;
  const Frustum& GetFrustum() const;
  XMVECTOR GetPosition() const;
  XMVECTOR GetForwardVector() const;
  XMVECTOR GetRightVector() const;
  XMVECTOR GetUpVector() const;

  XMVECTOR ViewToWorldPosition(FXMVECTOR p) const;
  XMVECTOR WorldToViewPosition(FXMVECTOR p) const;

protected:
  // Came coordinate system with coordinate relative to world space.
  XMVECTOR position_;
  XMVECTOR forward_vector_;

private:
  Frustum frustum_;
};

}  // namespace LL3D
