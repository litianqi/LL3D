#include "Graphics/Camera.h"
#include "Core/Assert.h"
#include "Transform.h"
#include "Graphics/Effects.h"

namespace LL3D {
namespace Graphics {

StaticCamera::StaticCamera(float fovY, float aspectRatio, float nearZ, float farZ) :
  fovY_(fovY),
  aspectRatio_(aspectRatio),
  nearZ_(nearZ),
  farZ_(farZ) {
  ASSERT(aspectRatio > 0);
  ASSERT(nearZ > 0);
  ASSERT(farZ > nearZ);
}

void StaticCamera::setAspectRatio(float aspectRatio) {
  aspectRatio_ = aspectRatio;
}

void StaticCamera::update()
{
  proj_ = Math::Matrix::CreatePerspectiveFieldOfView(
    fovY_,
    aspectRatio_,
    static_cast<float>(nearZ_),
    static_cast<float>(farZ_)
    );
}

const Math::Matrix& StaticCamera::projMaxtrix() const
{
  return proj_;
}


Camera::Camera(Transform & transform) :
  StaticCamera(Math::XM_PIDIV4, 16.0f / 9.0f, 1.f, 1000.0f),
  transform_(transform)
{
  ASSERT(transform.forwardVec() != Math::Vector3::Zero);
  ASSERT(!XMVector3IsInfinite(transform.forwardVec()));
  ASSERT(transform.forwardVec().Cross(transform.upVec()) != Math::Vector3::Zero);
}

const Math::Matrix& Camera::viewMatrix() const
{
  return view_;
}

const Math::Matrix& Camera::viewProjMatrix() const
{
  return viewProj_;
}

DirectX::BoundingFrustum Camera::frustum() const
{
  return frustum_;
}

void Camera::update()
{
  // Update proj matrix.
  StaticCamera::update();

  // Update view matrix.
  const auto pos = transform_.position();
  const auto forward = transform_.forwardVec();
  const auto up = transform_.upVec();
  view_ = Math::Matrix::CreateLookTo(pos, forward, up);

  // Update viewProj matrix.
  viewProj_ = viewMatrix() * projMaxtrix();

  // Update bounding frustum.
  auto frustum = DirectX::BoundingFrustum(projMaxtrix());
  frustum.Origin = transform_.position();
  frustum.Orientation = transform_.rotationQuaternion();
  frustum_ = frustum;
}

void Camera::writeToEffect() const
{
  s_effect->setEyePosW(transform_.position());
  s_effect->setViewProjection(viewProjMatrix());
}

}  // namespace Graphics
}  // namespace LL3D