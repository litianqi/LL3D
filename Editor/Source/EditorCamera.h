#pragma once

#include <Graphics/Camera.h>
#include <Math/Math.h>

using namespace DirectX;

class EditorCamera : public LL3D::Graphics::Camera {
public:
  EditorCamera(Frustum frustum, XMVECTOR position, XMVECTOR forward_vector);
  std::unique_ptr<Component> Clone() override;

  //////////////////////////////////////////////////////////////////////////
  /////////////////////////////// Move

  ///
  // Move d in world space.
  //
  void MoveLeftRight(float d);
  void MoveUpDown(float d);
  void MoveBackForeward(float d);

  //////////////////////////////////////////////////////////////////////////
  /////////////////////////////// Rotate

  ///
  // Rotate around right vector in target pos.
  //
  void Pitch(float angle);
  ///
  // Rotate around world up vector in target pos.
  //
  void Yaw(float angle);

  //////////////////////////////////////////////////////////////////////////
  /////////////////////////////// Operations

  ///
  // Update Camera properties based on user input. Then writes properties
  // to shader input.
  //
  void Update() override;

private:
  // Point where vec_target cross with XZ panel of world space.
  XMVECTOR GetTargetPosition() const;

  LL3D::Math::Vector2 last_mouse_position_;

};
