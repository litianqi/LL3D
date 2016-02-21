#pragma once

#include "Camera.h"

namespace LL3D {

class EditorCamera : public Camera {
public:
  EditorCamera(Frustum frustum, DirectX::XMVECTOR position, DirectX::XMVECTOR forward_vector);

  // Move d in world space.
  void MoveLeftRight(float d);
  void MoveUpDown(float d);
  void MoveBackForeward(float d);

  // Rotate around right vector in target pos.
  void Pitch(float angle);
  // Rotate around world up vector in target pos.
  void Yaw(float angle);

private:
  // Point where vec_target cross with XZ panel of world space.
  DirectX::XMVECTOR GetTargetPosition() const;
};

}  // namespace LL3D
