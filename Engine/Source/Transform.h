#pragma once

#include "Math\Math.h"
#include "Graphics\Base.h"
#include "Component.h"

namespace LL3D {

class Transform : public Component, private Graphics::Base {
public:

  Transform(const Transform* parent_transform);

  void setParentTransform(const Transform* parent_transform);
  void setLocalPosition(Math::Vector3 value);
  void setLocalRotation(Math::Vector3 value);
  void setLocalScale(Math::Vector3 value);
  void setLocalDirection(Math::Vector3 value);
  void setLocalMatrix(Math::Matrix value);
  void setPosition(Math::Vector3 value);
  void setRotation(Math::Vector3 value);
  void setScale(Math::Vector3 value);
  void setDirection(Math::Vector3 value);
  void setMatrix(Math::Matrix value);

  Math::Vector3 localPosition() const;
  Math::Vector3 localRotation() const;
  Math::Vector3 localScale() const;
  Math::Vector3 localUpVec() const;
  Math::Vector3 localRightVec() const;
  Math::Vector3 localForwardVec() const;
  Math::Matrix  composeMatrix() const;
  Math::Vector3 position() const;
  Math::Vector3 rotation() const;
  Math::Quaternion rotationQuaternion() const;
  Math::Vector3 scale() const;
  Math::Matrix  matrix() const;

  // Up vector of local space in world space.
  Math::Vector3 upVec() const;
  
  // Right vector of local space in world space.
  Math::Vector3 rightVec() const;
  
  // Forward vector of local space in world space.
  Math::Vector3 forwardVec() const;

  // Writes world matrix to shader buffer.
  void writeToEffect() const;
  static void writeToEffect(Math::Matrix world);

private:

  static Math::Matrix composeMatrix(Math::Vector3 position, Math::Vector3 rotation,
    Math::Vector3 scale);

  // transform of parent GameObject, nullptr if no parent.
  const Transform* parentTransform_;
  // local position
  Math::Vector3 position_;
  // local rotation
  Math::Vector3 rotation_;
  // local scale
  Math::Vector3 scale_ = Math::Vector3::One;

};

}  // namespace LL3D 
