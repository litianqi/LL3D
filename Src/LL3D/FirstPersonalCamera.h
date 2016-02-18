#pragma once

#include "Camera.h"

namespace LL3D {

class FirstPersonalCamera : public Camera {
public:
  FirstPersonalCamera(Frustum frustum, DirectX::XMVECTOR pos, DirectX::XMVECTOR vec_target);

  void MoveBackForeward(float d);
  void MoveUpDown(float d);
  void MoveLeftRight(float d);

  void Pitch(float radians);
  void Yaw(float radians);
};

}  // namespace LL3D
