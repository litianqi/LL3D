#pragma once

#include "Math\Math.h"
#include "Graphics\Base.h"
#include "Component.h"

namespace LL3D {

class Transform : public Component, private Graphics::Base {
public:
  std::unique_ptr<Component> Clone() override;

  
  //--------------------------------------
  // Properties

  void SetPosition(Math::Vector3 value);
  void SetRotation(Math::Vector3 value);
  void SetScale(Math::Vector3 value);
  void SetWorldPosition(Math::Vector3 value);
  void SetWorldRotation(Math::Vector3 value);
  void SetWorldScale(Math::Vector3 value);

  Math::Vector3 GetPosition() const;
  Math::Vector3 GetRotaion() const;
  Math::Vector3 GetScale() const;
  Math::Matrix  GetMatrix() const;
  Math::Vector3 GetWorldPosition() const;
  Math::Vector3 GetWorldRotation() const;
  Math::Vector3 GetWorldScale() const;
  Math::Matrix  GetWorldMatrix() const;

  
  //--------------------------------------
  // Operations

  //>
  // Writes world matrix to shader buffer.
  //
  void Update() override;

private:
  static Math::Matrix GetMatrix(Math::Vector3 position, Math::Vector3 rotation,
    Math::Vector3 scale);

  Math::Vector3 position_;
  Math::Vector3 rotation_;
  Math::Vector3 scale_ = Math::Vector3::One;
};

}  // namespace LL3D 
