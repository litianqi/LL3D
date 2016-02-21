#pragma once

#include <DirectXMath.h>

namespace LL3D {

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

    DirectX::XMMATRIX GetProjectionMaxtrix() const;
  
  private:
    float radian_fov_y_;
    float aspect_ratio_;
    float z_near_;
    float z_far_;
  };

  Camera(Frustum frustum, DirectX::FXMVECTOR position, DirectX::FXMVECTOR forward_vector);
  
  void SetFrustum(const Frustum& frustum);
  void SetPosition(DirectX::FXMVECTOR p);
  void SetForwardVector(DirectX::FXMVECTOR v);
 
  DirectX::XMMATRIX GetViewMatrix() const;
  DirectX::XMMATRIX GetViewProjectionMatrix() const;
  const Frustum& GetFrustum() const;
  DirectX::XMVECTOR GetPosition() const;
  DirectX::XMVECTOR GetForwardVector() const;
  DirectX::XMVECTOR GetRightVector() const;
  DirectX::XMVECTOR GetUpVector() const;

  DirectX::XMVECTOR ViewToWorldPosition(DirectX::FXMVECTOR p) const;
  DirectX::XMVECTOR WorldToViewPosition(DirectX::FXMVECTOR p) const;

protected:
  // Came coordinate system with coordinate relative to world space.
  DirectX::XMVECTOR position_;
  DirectX::XMVECTOR forward_vector_;

private:
  Frustum frustum_;
};

}  // namespace LL3D
