#include "EditorCameraController.h"
#include <Math/Vector.h>
#include <Math/Matrix.h>
#include <Core/Assert.h>
#include <Input/Mouse.h>
#include <Graphics/Camera.h>
#include <Component.h>
#include <Transform.h>
#include <GameObject.h>

using namespace LL3D;

std::unique_ptr<Component> EditorCameraController::Clone() {
  return std::unique_ptr<Component>(new EditorCameraController(*this));
}

void EditorCameraController::MoveLeftRight(float d) {
  auto normal_right = camera_->GetRightVector();
  normal_right.Normalize();
  SetPosition(GetPosition() + d * normal_right);
}

void EditorCameraController::MoveUpDown(float d) {
  auto normal_up = camera_->GetUpVector();
  normal_up.Normalize();
  SetPosition(GetPosition() + d * normal_up);
}

void EditorCameraController::MoveBackForeward(float d) {
  auto normal_forward = camera_->GetForwardVector();
  normal_forward.Normalize();
  SetPosition(GetPosition() + d * normal_forward);
}

void EditorCameraController::Pitch(float angle) {
  // Prevent target vector too close to (or even pass) up vector:
  
  // Get angle between target and up.
  auto cos_target_up = camera_->GetForwardVector().Dot(Math::Vector3::Up) / 
    camera_->GetForwardVector().Length();
  auto radian_target_up = acosf(cos_target_up);
  if (radian_target_up > Math::XM_PIDIV2)
    radian_target_up = Math::XM_PI - radian_target_up;
  if (radian_target_up > Math::XM_PIDIV4)
    radian_target_up = Math::XM_PIDIV2 - radian_target_up;
  
  // Correct angle.
  if (cos_target_up < 0 &&  // Target vector towards Up vector.
    angle > 0) {
    if (angle >= radian_target_up) {
      angle = radian_target_up - 0.002f;
    }
  }
  else if (((cos_target_up > 0 &&  // Target vector leaves Up vector.
    angle < 0))) {
    if (std::fabs(angle) >= radian_target_up) {
      angle = -(radian_target_up - 0.002f);
    }
  }
  
  auto matrix_rotate = XMMatrixRotationAxis(camera_->GetRightVector(), angle);

  // Change camera position:

  // Move target to origin
  auto target_pos = GetTargetPosition();
  auto position = GetPosition();
  position -= target_pos;

  // Rotate
  position = XMVector3Transform(position, matrix_rotate);
  
  // Move back
  position += target_pos;
  SetPosition(position);

  // Calulate new target vector.
  camera_->SetForwardVector(target_pos - position);
}

void EditorCameraController::Yaw(float angle) {
  auto matrix_rotate = XMMatrixRotationAxis(Math::Vector3{ 0, 1.0f, 0 }, angle);

  // Change camera position:
  
  // Move target to origin
  auto target_pos = GetTargetPosition();
  auto position = GetPosition();
  position -= target_pos;
  
  // Rotate
  position = XMVector3Transform(position, matrix_rotate);
  
  // Move back
  position += target_pos;
  SetPosition(position);

  // Calulate new target vector:
  camera_->SetForwardVector(target_pos - position);
}

void EditorCameraController::Start()
{
  camera_ = GetGameObject()->GetComponent<LL3D::Graphics::Camera>();
}

void EditorCameraController::Update() {
  auto delta = Input::Mouse::GetPosition() - last_mouse_position_;
  last_mouse_position_ = Input::Mouse::GetPosition();

  if (Input::Mouse::IsHeldingDown(Input::Mouse::Right)) {
    MoveLeftRight(-0.05f * delta.x);
    MoveUpDown(0.05f * delta.y);
  }
  MoveBackForeward(Input::Mouse::GetScrollDelta() * 0.02f);

  if (Input::Mouse::IsHeldingDown(Input::Mouse::Left)) {
    Yaw(0.005f * delta.x);
    Pitch(0.005f * delta.y);
  }
}

Math::Vector3 EditorCameraController::GetTargetPosition() const {
  Math::Vector3 world_origin = camera_->WorldToViewPosition(Math::Vector3::Zero);
  Math::Vector3 world_up = camera_->WorldToViewPosition(Math::Vector3::Up);
  
  float z_vec_v = -(world_origin.x * world_up.x + world_origin.y * world_up.y) / world_up.z;
  float target_z = z_vec_v + world_origin.z;

  Math::Vector3 target_pos(0, 0, target_z);
  
  return camera_->ViewToWorldPosition(target_pos);
}

Math::Vector3 EditorCameraController::GetPosition() const
{
  return GetGameObject()->GetComponent<Transform>()->GetPosition();
}

void EditorCameraController::SetPosition(Math::Vector3 value)
{
  GetGameObject()->GetComponent<Transform>()->SetPosition(value);
}
