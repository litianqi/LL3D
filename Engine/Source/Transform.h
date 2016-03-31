#pragma once

#include "Math\Math.h"
#include "Graphics\Base.h"
#include "Component.h"

namespace LL3D {

class Transform : public Component, private Graphics::Base {
public:
  std::unique_ptr<Component> Clone() override;

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

  Math::Vector3 GetLocalPosition() const;
  Math::Vector3 GetLocalRotation() const;
  Math::Vector3 GetLocalScale() const;
  Math::Vector3 GetLocalDirection() const;
  Math::Matrix  Compose() const;
  Math::Vector3 GetPosition() const;
  Math::Vector3 GetRotation() const;
  Math::Vector3 GetScale() const;
  Math::Vector3 GetDirection() const;
  Math::Matrix  GetMatrix() const;

  //>
  // Writes world matrix to shader buffer.
  //
  void Render();

private:
  static Math::Matrix Compose(Math::Vector3 position, Math::Vector3 rotation,
    Math::Vector3 scale);

  // local position
  Math::Vector3 position_;
  // local rotation
  Math::Vector3 rotation_;
  // local scale
  Math::Vector3 scale_ = Math::Vector3::One;
};

}  // namespace LL3D 
