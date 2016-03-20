#include "FirstPersonalCamera.h"
#include <Core/Assert.h>

using namespace DirectX;

namespace LL3D {
namespace Graphics {

FirstPersonalCamera::FirstPersonalCamera(Frustum frustum, XMVECTOR vec_target) :
  Camera(frustum, vec_target) {
}

void FirstPersonalCamera::MoveBackForeward(float d) {
  SetPosition(GetPosition() + d * XMVector3Normalize(forward_vector_));
}

void FirstPersonalCamera::MoveUpDown(float d) {
  SetPosition(GetPosition() + d * XMVECTOR{ 0, 1.0f });
}

void FirstPersonalCamera::MoveLeftRight(float d) {
  SetPosition(GetPosition() + d * XMVector3Normalize(GetRightVector()));
}

void FirstPersonalCamera::Pitch(float radians) {
  XMMATRIX matrix = XMMatrixRotationAxis(GetRightVector(), radians);
  forward_vector_ = XMVector3Transform(forward_vector_, matrix);
}

void FirstPersonalCamera::Yaw(float radians) {
  XMMATRIX matrix = XMMatrixRotationAxis(GetUpVector(), radians);
  forward_vector_ = XMVector3Transform(forward_vector_, matrix);
}

}  // namespace Graphics
}  // namespace LL3D
