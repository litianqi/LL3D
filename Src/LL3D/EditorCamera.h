#pragma once

#include "Camera.h"

namespace LL3D {

class EditorCamera : public Camera {
public:
  EditorCamera(Frustum frustum, DirectX::XMVECTOR pos,
    DirectX::XMVECTOR vec_target, DirectX::XMVECTOR vec_up);

  void MoveLeftRight(float d);
  void MoveUpDown(float d);
  void MoveBackForeward(float d);

  void Pitch(float radians);
  void Yaw(float radians);

private:
  DirectX::XMVECTOR GetTargetPos() const;
};

}  // namespace LL3D
