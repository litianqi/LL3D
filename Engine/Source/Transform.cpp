#include "Transform.h"
#include "Graphics\Effects.h"
#include "GameObject.h"
#include "Core\Assert.h"

namespace LL3D {
Transform::Transform(const Transform * parent_transform) :
  parentTransform_(parent_transform)
{
}

void Transform::setParentTransform(const Transform * parent_transform)
{
  parentTransform_ = parent_transform;
}

void Transform::setLocalPosition(Math::Vector3 value)
{
  position_ = value;
}

void Transform::setLocalRotation(Math::Vector3 value) 
{
  rotation_ = value;
}

void Transform::setLocalScale(Math::Vector3 value) 
{
  scale_ = value;
}

void Transform::setLocalMatrix(Math::Matrix value)
{
  auto scale = Math::Vector3();
  auto roatation = Math::Quaternion();
  auto position= Math::Vector3();
  bool s = value.Decompose(scale, roatation, position);
  if (!s)
    throw std::invalid_argument("Cann't decompose argument.");

  scale_ = scale;
  rotation_ = roatation.GetPitchYawRollVector();
  position_ = position;
}

void Transform::setPosition(Math::Vector3 value) 
{
  if (parentTransform_)
    position_ = value - parentTransform_->position();
  else
    position_ = value;
}

void Transform::setRotation(Math::Vector3 value) 
{
  if (parentTransform_)
    setLocalRotation(value - parentTransform_->rotation());
  else
    setLocalRotation(value);
}

void Transform::setScale(Math::Vector3 value) 
{
  if (parentTransform_)
    scale_ = value / parentTransform_->scale();
  else
    scale_ = value;
}

void Transform::setMatrix(Math::Matrix value)
{
  auto matrix = Math::Matrix();
  if (parentTransform_)
    matrix = value / parentTransform_->matrix();
  else
    matrix = value;
  setLocalMatrix(matrix);
}

Math::Vector3 Transform::localPosition() const 
{
  return position_;
}

Math::Vector3 Transform::localRotation() const 
{
  return rotation_;
}

Math::Vector3 Transform::localScale() const 
{
  return scale_;
}

Math::Vector3 Transform::localUpVec() const
{
  auto rotation = localRotation();
  auto matrix = Math::Matrix::CreateFromYawPitchRoll(rotation.y, rotation.x, rotation.z);
  return Math::Vector3::TransformNormal(Math::Vector3::Up, matrix);
}

Math::Vector3 Transform::localRightVec() const
{
  auto rotation = localRotation();
  auto matrix = Math::Matrix::CreateFromYawPitchRoll(rotation.y, rotation.x, rotation.z);
  return Math::Vector3::TransformNormal(Math::Vector3::Right, matrix);
}

Math::Vector3 Transform::localForwardVec() const
{
  auto rotation = localRotation();
  auto matrix = Math::Matrix::CreateFromYawPitchRoll(rotation.y, rotation.x, rotation.z);
  return Math::Vector3::TransformNormal(Math::Vector3::Forward, matrix);
}

Math::Matrix Transform::composeMatrix() const 
{
  return composeMatrix(position_, rotation_, scale_);
}

Math::Vector3 Transform::position() const 
{
  if (parentTransform_)
    return position_ + parentTransform_->position();
  else
    return position_;
}

Math::Vector3 Transform::rotation() const 
{
  if (parentTransform_)
    return rotation_ + parentTransform_->rotation();
  else
    return rotation_;
}

Math::Quaternion Transform::rotationQuaternion() const
{
  return Math::Quaternion::CreateFromYawPitchRoll(
    rotation_.y, rotation_.x, rotation_.z
    );
}

Math::Vector3 Transform::scale() const 
{
  if (parentTransform_)
    return scale_ + parentTransform_->scale();
  else
    return scale_;
}

Math::Vector3 Transform::upVec() const
{
  if (parentTransform_) {
    ASSERT(false);
    // TODO
    return localUpVec();
  }    
  else {
    return localUpVec();
  }
}

Math::Vector3 Transform::rightVec() const
{
  if (parentTransform_) {
    ASSERT(false);
    // TODO
    return localRightVec();
  }
  else
    return localRightVec();
}

Math::Vector3 Transform::forwardVec() const
{
  if (parentTransform_) {
    ASSERT(false);
    // TODO
    return localForwardVec();
  }
  else
    return localForwardVec();
}

Math::Matrix Transform::matrix() const 
{
  return composeMatrix(position(), rotation(), scale());
}

void Transform::writeToEffect() const
{
  writeToEffect(matrix());
}

void Transform::writeToEffect(Math::Matrix world)
{
  s_effect->setWorld(world);
}

Math::Matrix Transform::composeMatrix(Math::Vector3 position, Math::Vector3 rotation, Math::Vector3 scale) 
{
  auto r = Math::Matrix::CreateFromYawPitchRoll(rotation.y, rotation.x, rotation.z);
  auto s = Math::Matrix::CreateScale(scale);
  auto t = Math::Matrix::CreateTranslation(position);
  auto tmp = r * s * t;
  return r * s * t;
}

}  // namespace LL3D 