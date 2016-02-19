#pragma once

#include <DirectXMath.h>

namespace LL3D {

// Non-rollable Camera. 
// Non-rollable means Up and Target vector always on a YZ panel(of world space) 
// roated some degree around Y.

// Terms:
// Target = Z-Negative (view space);
// Up = Y-Positive; Right = X-Positive.

class Camera {
public:
  // Frustum properties of a Camera.
  // Can generate a Projection Matrix.
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

  Camera(Frustum frustum, DirectX::XMVECTOR pos, DirectX::XMVECTOR vec_target);
  
  void SetFrustum(const Frustum& frustum);
  void SetPos(DirectX::FXMVECTOR pos);
  void SetTargetPos(DirectX::FXMVECTOR pos_target);
 
  DirectX::XMMATRIX GetViewMatrix() const;
  DirectX::XMMATRIX GetViewProjectionMatrix() const;
  const Frustum& GetFrustum() const;
  DirectX::XMVECTOR GetPos() const;

  DirectX::XMVECTOR ViewToWorldPos(DirectX::FXMVECTOR pos) const;
  DirectX::XMVECTOR WorldToViewPos(DirectX::FXMVECTOR pos) const;

protected:
  DirectX::XMVECTOR GetRightVector() const;
  DirectX::XMVECTOR GetUpVector() const;

  // Came coordinate system with coordinate relative to world space.
  DirectX::XMVECTOR pos_;
  DirectX::XMVECTOR vec_target_;

private:
  Frustum frustum_;
};

}  // namespace LL3D
