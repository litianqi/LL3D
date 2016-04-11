#include "Graphics/Camera.h"
#include "Core/Assert.h"
#include "GameObject.h"
#include "Transform.h"
#include "Graphics/Effects.h"

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

void Camera::Frustum::setAspectRatio(float aspect_ratio) {
  aspect_ratio_ = aspect_ratio;
}

Math::Matrix Camera::Frustum::projectionMaxtrix() const 
{
  return Math::Matrix::CreatePerspectiveFieldOfView(radian_fov_y_, aspect_ratio_,
    static_cast<float>(z_near_), static_cast<float>(z_far_));
}

Camera::Camera(Transform& transform, Frustum frustum, Math::Vector3 forward_vector) :
  transform_(transform),
  frustum_(frustum),
  forward_vector_(forward_vector) 
{
  ASSERT(forward_vector != Math::Vector3::Zero);
  ASSERT(!XMVector3IsInfinite(forward_vector));
  ASSERT(forward_vector.Cross(Math::Vector3::Up) != Math::Vector3::Zero);
}

void Camera::render() const 
{
  s_effect->setEyePosW(GetPosition());
  s_effect->setViewProjection(viewMatrix() * frustum().projectionMaxtrix());
}

void Camera::SetFrustum(const Frustum & frustum) 
{
  frustum_ = frustum;
}

void Camera::SetPosition(Math::Vector3 p) 
{
  transform_.setPosition(p);
}

void Camera::SetForwardVector(Math::Vector3 v) 
{
  forward_vector_ = v;
}

Math::Matrix Camera::viewMatrix() const 
{
  return Math::Matrix::CreateLookTo(GetPosition(), forward_vector_, Math::Vector3{ 0.f, 1.f, 0.f });
}

Math::Matrix Camera::viewProjectionMatrix() const 
{
  Math::Matrix projection = frustum_.projectionMaxtrix();
  return viewMatrix() * projection;
}

const Camera::Frustum& Camera::frustum() const 
{
  return frustum_;
}

Math::Vector3 Camera::GetPosition() const 
{
  return transform_.position();
}

Math::Vector3 Camera::GetForwardVector() const 
{
  return forward_vector_;
}

Math::Vector3 Camera::convertViewToWorld(Math::Vector3 position) const 
{
  auto view = viewMatrix();
  return Math::Vector3::Transform(position, view.Invert());
}

Math::Vector3 Camera::convertWorldToView(Math::Vector3 position) const 
{
  return XMVector3Transform(position, viewMatrix());
}

Math::Vector3 Camera::GetRightVector() const {
  return XMVector3Cross(Math::Vector3::Up, forward_vector_);
}

Math::Vector3 Camera::GetUpVector() const {
  float y = forward_vector_.LengthSquared() /
    forward_vector_.Dot(Math::Vector3::Up);
  Math::Vector3 v{ 0, y, 0 };

  return forward_vector_ - v;
}

DirectX::BoundingFrustum Camera::boundingFrustum() const
{
  auto localFrustum = DirectX::BoundingFrustum(
    frustum().projectionMaxtrix()
    );
  // local -> world
 /* localFrustum.Origin = GetPosition();
  auto yawPitchRoll = GetForwardVector()- Math::Vector3::Up;
  localFrustum.Orientation = Math::Quaternion::CreateFromYawPitchRoll();*/
  return localFrustum;
}

}  // namespace Graphics
}  // namespace LL3D
