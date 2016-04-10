#pragma once

#include "Math\Math.h"
#include "Graphics\Base.h"
#include "Component.h"

namespace LL3D {

class Transform : public Component, private Graphics::Base {
public:
  Transform(const Transform* parent_transform);

  void setParentTransform(const Transform* parent_transform);
  void SetLocalPosition(Math::Vector3 value);
  void SetLocalRotation(Math::Vector3 value);
  void SetLocalScale(Math::Vector3 value);
  void SetLocalDirection(Math::Vector3 value);
  void SetLocalMatrix(Math::Matrix value);
  void SetPosition(Math::Vector3 value);
  void SetRotation(Math::Vector3 value);
  void SetScale(Math::Vector3 value);
  void SetDirection(Math::Vector3 value);
  void SetMatrix(Math::Matrix value);

  Math::Vector3 localPosition() const;
  Math::Vector3 localRotation() const;
  Math::Vector3 localScale() const;
  Math::Vector3 localDirection() const;
  Math::Matrix  compose() const;
  Math::Vector3 position() const;
  Math::Vector3 rotation() const;
  Math::Quaternion rotationQuaternion() const;
  Math::Vector3 scale() const;
  Math::Vector3 direction() const;
  Math::Matrix  matrix() const;

  //>
  // Writes world matrix to shader buffer.
  //
  void Render() const;
  static void Render(Math::Matrix world);

private:
  static Math::Matrix compose(Math::Vector3 position, Math::Vector3 rotation,
    Math::Vector3 scale);

  // transform of parent GameObject, nullptr if no parent.
  const Transform* parent_transform_;
  // local position
  Math::Vector3 position_;
  // local rotation
  Math::Vector3 rotation_;
  // local scale
  Math::Vector3 scale_ = Math::Vector3::One;
};

}  // namespace LL3D 
