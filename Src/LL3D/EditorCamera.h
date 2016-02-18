#pragma once

#include "Camera.h"

namespace LL3D {

class EditorCamera : public Camera {
public:
  EditorCamera(Frustum frustum, DirectX::XMVECTOR pos, DirectX::XMVECTOR vec_target);

  // Move d in world space.
  void MoveLeftRight(float d);
  void MoveUpDown(float d);
  void MoveBackForeward(float d);

  // Rotate around right vector in target pos.
  void Pitch(float radians);
  // Rotate around world up vector in target pos.
  void Yaw(float radians);

private:
  // Point where vec_target cross with XZ panel of world space.
  DirectX::XMVECTOR GetTargetPos() const;
};

}  // namespace LL3D
