#pragma once

#include <Behaviour.h>
#include <Math/Math.h>

namespace LL3D {
class GameObject;
namespace Graphics {
class Camera;
}
}

class EditorCameraController : public LL3D::Behaviour {
public:
  EditorCameraController(LL3D::GameObject* object) : Behaviour(object) {}
  
  // Move d in world space.
  void MoveLeftRight(float d);
  void MoveUpDown(float d);
  void MoveBackForeward(float d);
  
  // Rotate around right vector in target pos.
  void Pitch(float angle);
  // Rotate around world up vector in target pos.
  void Yaw(float angle);

  // Operations
  void Start() override;
  // Update Camera properties based on user input. Then writes properties
  // to shader input.
  void Update() override;

private:
  // Point where vec_target cross with XZ panel of world space.
  LL3D::Math::Vector3 GetTargetPosition() const;
  LL3D::Math::Vector3 GetPosition() const;
  void SetPosition(LL3D::Math::Vector3 value);

  LL3D::Graphics::Camera* camera_;
  LL3D::Math::Vector2 last_mouse_position_;
};
