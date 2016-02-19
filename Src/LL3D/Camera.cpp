#include "Camera.h"
#include "Debug.h"

using namespace DirectX;

namespace LL3D {

Camera::Frustum::Frustum(float radian_fov_y, float aspect_ratio, float z_near, float z_far) :
  radian_fov_y_(radian_fov_y),
  aspect_ratio_(aspect_ratio),
  z_near_(z_near),
  z_far_(z_far)
{
  Assert(aspect_ratio > 0);
  Assert(z_near > 0);
  Assert(z_far > z_near);
}

void Camera::Frustum::SetAspectRatio(float aspect_ratio) {
  aspect_ratio_ = aspect_ratio;
}

DirectX::XMMATRIX Camera::Frustum::GetProjectionMaxtrix() const {
  return DirectX::XMMatrixPerspectiveFovLH(radian_fov_y_, aspect_ratio_,
    static_cast<float>(z_near_), static_cast<float>(z_far_));
}

Camera::Camera(Frustum frustum, DirectX::XMVECTOR pos, DirectX::XMVECTOR vec_target) :
  frustum_(frustum),
  pos_(pos),
  vec_target_(vec_target)
{
  Assert(!XMVector3Equal(vec_target, XMVectorZero()));
  Assert(!XMVector3IsInfinite(vec_target));
  Assert(!XMVector3Equal(XMVector3Cross(vec_target, XMVECTOR{ 0, 1.0f }), XMVectorZero()));
}

void Camera::SetFrustum(const Frustum & frustum) {
  frustum_ = frustum;
}

void Camera::SetPos(DirectX::FXMVECTOR pos) {
  pos_ = pos;
}

void Camera::SetTargetPos(DirectX::FXMVECTOR pos_target) {
  vec_target_ = pos_target - pos_;
}

DirectX::XMMATRIX Camera::GetViewMatrix() const {
  return DirectX::XMMatrixLookToLH(pos_, vec_target_, XMVECTOR{ 0, 1.0f });;
}

DirectX::XMMATRIX Camera::GetViewProjectionMatrix() const {
  DirectX::XMMATRIX projection = frustum_.GetProjectionMaxtrix();
  return GetViewMatrix() * projection;
}

const Camera::Frustum& Camera::GetFrustum() const {
  return frustum_;
}

DirectX::XMVECTOR Camera::GetPos() const {
  return pos_;
}

XMVECTOR Camera::ViewToWorldPos(FXMVECTOR pos) const {
  auto matrix_view = GetViewMatrix();
  auto matrix = XMMatrixInverse(&XMMatrixDeterminant(matrix_view), matrix_view);
  return XMVector3Transform(pos, matrix);
}

DirectX::XMVECTOR Camera::WorldToViewPos(DirectX::FXMVECTOR pos) const {
  return XMVector3Transform(pos, GetViewMatrix());
}

DirectX::XMVECTOR Camera::GetRightVector() const {
  return XMVector3Cross(vec_target_, XMVECTOR{0, 1.0f});
}

DirectX::XMVECTOR Camera::GetUpVector() const {
  // Ask: vec_up

  // Answer:
  // vec_up = pos_y - pos_target, with pos_target = vec_target
  // pos_y = {0, y, 0}
  // length(vec_target) / y = cos
  // vec_target * {0, 1, 0} = |vec_target| * |{0, 1, 0}| * cos

  // Program Answer:

  float y = XMVectorGetX(XMVector3LengthSq(vec_target_)) / XMVectorGetX(XMVector3Dot(vec_target_, XMVECTOR{ 0, 1.0f }));
  XMVECTOR pos_y{ 0, y, 0, 1.0f };

  return pos_y - vec_target_;
}

}  // namespace LL3D
