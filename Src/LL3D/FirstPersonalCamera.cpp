#include "FirstPersonalCamera.h"
#include "Debug.h"

using namespace DirectX;

namespace LL3D {

FirstPersonalCamera::FirstPersonalCamera(Frustum frustum, DirectX::XMVECTOR pos, DirectX::XMVECTOR vec_target) :
  Camera(frustum, pos, vec_target) {
}

void FirstPersonalCamera::MoveBackForeward(float d) {
  position_ += d * XMVector3Normalize(forward_vector_);
}

void FirstPersonalCamera::MoveUpDown(float d) {
  position_ += d * XMVECTOR{ 0, 1.0f };
}

void FirstPersonalCamera::MoveLeftRight(float d) {
  position_ += d * XMVector3Normalize(GetRightVector());
}

void FirstPersonalCamera::Pitch(float radians) {
  DirectX::XMMATRIX matrix = DirectX::XMMatrixRotationAxis(GetRightVector(), radians);
  forward_vector_ = XMVector3Transform(forward_vector_, matrix);
}

void FirstPersonalCamera::Yaw(float radians) {
  DirectX::XMMATRIX matrix = DirectX::XMMatrixRotationAxis(GetUpVector(), radians);
  forward_vector_ = XMVector3Transform(forward_vector_, matrix);
}

}  // namespace LL3D
