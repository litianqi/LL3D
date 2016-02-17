#include "FirstPersonalCamera.h"
#include "Debug.h"

using namespace DirectX;

namespace LL3D {

FirstPersonalCamera::FirstPersonalCamera(Frustum frustum, DirectX::XMVECTOR pos, DirectX::XMVECTOR vec_target, DirectX::XMVECTOR vec_up) :
  Camera(frustum, pos, vec_target, vec_up) {
}

void FirstPersonalCamera::MoveBackForeward(float d) {
  pos_ += d * XMVector3Normalize(vec_target_);
}

void FirstPersonalCamera::MoveUpDown(float d) {
}

void FirstPersonalCamera::MoveLeftRight(float d) {
  pos_ += d * XMVector3Normalize(GetRightVector());
}

void FirstPersonalCamera::Pitch(float radians) {
  DirectX::XMMATRIX matrix_rotate = DirectX::XMMatrixRotationAxis(GetRightVector(), radians);
  vec_target_ = XMVector3TransformNormal(vec_target_, matrix_rotate);
  vec_up_ = XMVector3TransformNormal(vec_up_, matrix_rotate);
}

void FirstPersonalCamera::Yaw(float radians) {
  DirectX::XMMATRIX matrix_rotate = DirectX::XMMatrixRotationAxis(vec_up_, -radians);
  vec_target_ = XMVector3TransformNormal(vec_target_, matrix_rotate);
}

}  // namespace LL3D
