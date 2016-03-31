#include "Transform.h"
#include "Graphics\Effects.h"
#include "GameObject.h"

namespace LL3D {

std::unique_ptr<Component> Transform::Clone() {
  return std::make_unique<Transform>(*this);
}

void Transform::Render() {
  s_effect->SetWorld(GetMatrix());
}

void Transform::SetLocalPosition(Math::Vector3 value) {
  position_ = value;
}

void Transform::SetLocalRotation(Math::Vector3 value) {
  rotation_ = value;
}

void Transform::SetLocalScale(Math::Vector3 value) {
  scale_ = value;
}

void Transform::SetLocalMatrix(Math::Matrix value)
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

void Transform::SetPosition(Math::Vector3 value) {
  auto parent = GetGameObject()->GetParent();
  if (parent)
    position_ = value - parent->GetComponent<Transform>()->GetPosition();
  else
    position_ = value;
}

void Transform::SetRotation(Math::Vector3 value) {
  auto parent = GetGameObject()->GetParent();
  if (parent)
    SetLocalRotation(value - parent->GetComponent<Transform>()->GetRotation());
  else
    SetLocalRotation(value);
}

void Transform::SetScale(Math::Vector3 value) {
  auto parent = GetGameObject()->GetParent();
  if (parent)
    scale_ = value / parent->GetComponent<Transform>()->GetScale();
  else
    scale_ = value;
}

void Transform::SetMatrix(Math::Matrix value)
{
  auto matrix = Math::Matrix();
  auto parent = GetGameObject()->GetParent();
  if (parent)
    matrix = value / parent->GetComponent<Transform>()->GetMatrix();
  else
    matrix = value;
  SetLocalMatrix(matrix);
}

Math::Vector3 Transform::GetLocalPosition() const {
  return position_;
}

Math::Vector3 Transform::GetLocalRotation() const {
  return rotation_;
}

Math::Vector3 Transform::GetLocalScale() const {
  return scale_;
}

Math::Vector3 Transform::GetLocalDirection() const
{
  auto rotation = GetLocalRotation();
  auto matrix = Math::Matrix::CreateFromYawPitchRoll(rotation.y, rotation.x, rotation.z);
  return Math::Vector3::Transform(Math::Vector3::Up, matrix);
}

Math::Matrix Transform::Compose() const {
  return Compose(position_, rotation_, scale_);
}

Math::Vector3 Transform::GetPosition() const {
  auto parent = GetGameObject()->GetParent();
  if (parent)
    return position_ + parent->GetComponent<Transform>()->GetPosition();
  else
    return position_;
}

Math::Vector3 Transform::GetRotation() const {
  auto parent = GetGameObject()->GetParent();
  if (parent)
    return rotation_ + parent->GetComponent<Transform>()->GetRotation();
  else
    return rotation_;
}

Math::Vector3 Transform::GetScale() const {
  auto parent = GetGameObject()->GetParent();
  if (parent)
    return scale_ + parent->GetComponent<Transform>()->GetScale();
  else
    return scale_;
}

Math::Vector3 Transform::GetDirection() const
{
  auto parent = GetGameObject()->GetParent();
  if (parent)
    return GetLocalDirection() + parent->GetComponent<Transform>()->GetDirection();
  else
    return GetLocalDirection();
}

Math::Matrix Transform::GetMatrix() const {
  return Compose(GetPosition(), GetRotation(), GetScale());
}

Math::Matrix Transform::Compose(Math::Vector3 position, Math::Vector3 rotation, Math::Vector3 scale) {
  auto r = Math::Matrix::CreateFromYawPitchRoll(rotation.y, rotation.x, rotation.z);
  auto s = Math::Matrix::CreateScale(scale);
  auto t = Math::Matrix::CreateTranslation(position);
  return r * s * t;
}

}  // namespace LL3D 