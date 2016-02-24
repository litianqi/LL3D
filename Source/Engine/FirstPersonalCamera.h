#pragma once

#include "Camera.h"

namespace LL3D {

class FirstPersonalCamera : public Camera {
public:
  FirstPersonalCamera(Frustum frustum, XMVECTOR pos, XMVECTOR vec_target);

  void MoveBackForeward(float d);
  void MoveUpDown(float d);
  void MoveLeftRight(float d);

  void Pitch(float angle);
  void Yaw(float angle);
};

}  // namespace LL3D
