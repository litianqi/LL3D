#pragma once

#include <DirectXMath.h>

namespace LL3D {

class Camera {
public:
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

  Camera(Frustum frustum, DirectX::XMVECTOR pos, DirectX::XMVECTOR vec_target, DirectX::XMVECTOR vec_up);
  
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

  // Came coordinate system with coordinate relative to world space.
  DirectX::XMVECTOR pos_;
  DirectX::XMVECTOR vec_target_;
  DirectX::XMVECTOR vec_up_;
  // uvec_right can be calculated from uvec_target and uvec_up.

private:
  Frustum frustum_;
};

}  // namespace LL3D
