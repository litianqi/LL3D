#pragma once

#include "Camera.h"

namespace LL3D {
namespace Graphics {

class FirstPersonalCamera : public Camera {
public:
  FirstPersonalCamera(Frustum frustum, DirectX::XMVECTOR pos, DirectX::XMVECTOR vec_target);

  void MoveBackForeward(float d);
  void MoveUpDown(float d);
  void MoveLeftRight(float d);

  void Pitch(float angle);
  void Yaw(float angle);
};

}  // namespace Graphics
}  // namespace LL3D
