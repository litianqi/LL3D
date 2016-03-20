#include "Camera.h"
#include "../Core/Assert.h"
#include "../GameObject.h"
#include "../Transform.h"
#include "Effects.h"

using namespace DirectX;

namespace LL3D {
namespace Graphics {

Camera::Frustum::Frustum(float radian_fov_y, float aspect_ratio, float z_near, float z_far) :
  radian_fov_y_(radian_fov_y),
  aspect_ratio_(aspect_ratio),
  z_near_(z_near),
  z_far_(z_far) {
  ASSERT(aspect_ratio > 0);
  ASSERT(z_near > 0);
  ASSERT(z_far > z_near);
}

void Camera::Frustum::SetAspectRatio(float aspect_ratio) {
  aspect_ratio_ = aspect_ratio;
}

XMMATRIX Camera::Frustum::GetProjectionMaxtrix() const {
  return XMMatrixPerspectiveFovLH(radian_fov_y_, aspect_ratio_,
    static_cast<float>(z_near_), static_cast<float>(z_far_));
}

Camera::Camera(Frustum frustum, FXMVECTOR forward_vector) :
  frustum_(frustum),
  forward_vector_(forward_vector) {
  ASSERT(!XMVector3Equal(forward_vector, XMVectorZero()));
  ASSERT(!XMVector3IsInfinite(forward_vector));
  ASSERT(!XMVector3Equal(XMVector3Cross(forward_vector, XMVECTOR{ 0, 1.0f }), XMVectorZero()));
}

std::unique_ptr<Component> Camera::Clone() {
  return std::make_unique<Camera>(*this);
}

void Camera::Update() {
  s_effect->SetEyePosW(GetPosition());
  s_effect->SetView(GetViewMatrix());
  s_effect->SetProjection(GetFrustum().GetProjectionMaxtrix());
}

void Camera::SetFrustum(const Frustum & frustum) {
  frustum_ = frustum;
}

void Camera::SetPosition(FXMVECTOR p) {
  GetGameObject()->GetComponent<Transform>()->SetWorldPosition(p);
}

void Camera::SetForwardVector(FXMVECTOR v) {
  forward_vector_ = v;
}

XMMATRIX Camera::GetViewMatrix() const {
  return XMMatrixLookToLH(GetPosition(), forward_vector_, XMVECTOR{ 0, 1.0f });;
}

XMMATRIX Camera::GetViewProjectionMatrix() const {
  XMMATRIX projection = frustum_.GetProjectionMaxtrix();
  return GetViewMatrix() * projection;
}

const Camera::Frustum& Camera::GetFrustum() const {
  return frustum_;
}

XMVECTOR Camera::GetPosition() const {
  return GetGameObject()->GetComponent<Transform>()->GetWorldPosition();
}

XMVECTOR Camera::GetForwardVector() const {
  return forward_vector_;
}

XMVECTOR Camera::ViewToWorldPosition(FXMVECTOR p) const {
  auto matrix_view = GetViewMatrix();
  auto matrix = XMMatrixInverse(&XMMatrixDeterminant(matrix_view), matrix_view);
  return XMVector3Transform(p, matrix);
}

XMVECTOR Camera::WorldToViewPosition(FXMVECTOR p) const {
  return XMVector3Transform(p, GetViewMatrix());
}

XMVECTOR Camera::GetRightVector() const {
  return XMVector3Cross(XMVECTOR{ 0, 1.0f }, forward_vector_);
}

XMVECTOR Camera::GetUpVector() const {
  // Ask: vec_up

  // Answer:
  // vec_up = pos_y - pos_target, with pos_target = forward_vector
  // pos_y = {0, y, 0}
  // length(forward_vector) / y = cos
  // forward_vector * {0, 1, 0} = |forward_vector| * |{0, 1, 0}| * cos

  // Program Answer:

  float y = XMVectorGetX(XMVector3LengthSq(forward_vector_)) /
    XMVectorGetX(XMVector3Dot(forward_vector_, XMVECTOR{ 0, 1.0f }));
  XMVECTOR v{ 0, y, 0 };

  return forward_vector_ - v;
}

}  // namespace Graphics
}  // namespace LL3D
