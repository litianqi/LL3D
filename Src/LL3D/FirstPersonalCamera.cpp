#include "FirstPersonalCamera.h"
#include "Debug.h"

using namespace DirectX;

namespace LL3D {

FirstPersonalCamera::FirstPersonalCamera(Frustum frustum, DirectX::XMVECTOR pos, DirectX::XMVECTOR vec_target) :
  Camera(frustum, pos, vec_target) {
}

void FirstPersonalCamera::MoveBackForeward(float d) {
  pos_ += d * XMVector3Normalize(vec_target_);
}

void FirstPersonalCamera::MoveUpDown(float d) {
  pos_ += d * XMVECTOR{ 0, 1.0f };
}

void FirstPersonalCamera::MoveLeftRight(float d) {
  pos_ += d * XMVector3Normalize(GetRightVector());
}

void FirstPersonalCamera::Pitch(float radians) {
  DirectX::XMMATRIX matrix = DirectX::XMMatrixRotationAxis(GetRightVector(), radians);
  vec_target_ = XMVector3Transform(vec_target_, matrix);
}

void FirstPersonalCamera::Yaw(float radians) {
  DirectX::XMMATRIX matrix = DirectX::XMMatrixRotationAxis(GetUpVector(), radians);
  vec_target_ = XMVector3Transform(vec_target_, matrix);
}

}  // namespace LL3D
