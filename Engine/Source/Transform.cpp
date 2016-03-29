#include "Transform.h"
#include "Graphics\Effects.h"
#include "GameObject.h"

namespace LL3D {

std::unique_ptr<Component> Transform::Clone() {
  return std::make_unique<Transform>(*this);
}

void Transform::Update() {
  s_effect->SetWorld(GetWorldMatrix());
}

void Transform::SetPosition(Math::Vector3 value) {
  position_ = value;
}

void Transform::SetRotation(Math::Vector3 value) {
  rotation_ = value;
}

void Transform::SetScale(Math::Vector3 value) {
  scale_ = value;
}

void Transform::SetWorldPosition(Math::Vector3 value) {
  auto parent = GetGameObject()->GetParent();
  if (parent)
    position_ = value - parent->GetComponent<Transform>()->GetWorldPosition();
  else
    position_ = value;
}

void Transform::SetWorldRotation(Math::Vector3 value) {
  auto parent = GetGameObject()->GetParent();
  if (parent)
    rotation_ = value - parent->GetComponent<Transform>()->GetWorldRotation();
  else
    rotation_ = value;
}

void Transform::SetWorldScale(Math::Vector3 value) {
  auto parent = GetGameObject()->GetParent();
  if (parent)
    scale_ = value - parent->GetComponent<Transform>()->GetWorldScale();
  else
    scale_ = value;
}

Math::Vector3 Transform::GetPosition() const {
  return position_;
}

Math::Vector3 Transform::GetRotaion() const {
  return rotation_;
}

Math::Vector3 Transform::GetScale() const {
  return scale_;
}

Math::Matrix Transform::GetMatrix() const {
  return GetMatrix(position_, rotation_, scale_);
}

Math::Vector3 Transform::GetWorldPosition() const {
  auto parent = GetGameObject()->GetParent();
  if (parent)
    return position_ + parent->GetComponent<Transform>()->GetWorldPosition();
  else
    return position_;
}

Math::Vector3 Transform::GetWorldRotation() const {
  auto parent = GetGameObject()->GetParent();
  if (parent)
    return rotation_ + parent->GetComponent<Transform>()->GetWorldRotation();
  else
    return rotation_;
}

Math::Vector3 Transform::GetWorldScale() const {
  auto parent = GetGameObject()->GetParent();
  if (parent)
    return scale_ + parent->GetComponent<Transform>()->GetWorldScale();
  else
    return scale_;
}

Math::Matrix Transform::GetWorldMatrix() const {
  return GetMatrix(GetWorldPosition(), GetWorldRotation(), GetWorldScale());
}

Math::Matrix Transform::GetMatrix(Math::Vector3 position, Math::Vector3 rotation, Math::Vector3 scale) {
  auto r = Math::Matrix::CreateFromYawPitchRoll(rotation.y, rotation.x, rotation.z);
  auto s = Math::Matrix::CreateScale(scale);
  auto t = Math::Matrix::CreateTranslation(position);
  return r * s * t;
}

}  // namespace LL3D 