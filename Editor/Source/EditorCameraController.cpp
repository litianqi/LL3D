#include "EditorCameraController.h"
#include <Math/Math.h>
#include <Core/Assert.h>
#include <Input/Mouse.h>
#include <Graphics/Camera.h>
#include <Component.h>
#include <Transform.h>
#include <GameObject.h>

using namespace LL3D;
using namespace DirectX;

std::unique_ptr<Component> EditorCameraController::Clone() {
  return std::unique_ptr<Component>(new EditorCameraController(*this));
}

void EditorCameraController::MoveLeftRight(float d) {
  SetPosition(GetPosition() + d * XMVector3Normalize(camera_->GetRightVector()));
}

void EditorCameraController::MoveUpDown(float d) {
  SetPosition(GetPosition() + d * XMVector3Normalize(camera_->GetUpVector()));
}

void EditorCameraController::MoveBackForeward(float d) {
  SetPosition(GetPosition() + d * XMVector3Normalize(camera_->GetForwardVector()));
}

void EditorCameraController::Pitch(float angle) {
  // Prevent target vector too close to (or even pass) up vector:
  
  // Get angle between target and up.
  auto cos_target_up = XMVectorGetX(XMVector3Dot(camera_->GetForwardVector(), 
    XMVECTOR{ 0, 1.0f })) / XMVectorGetX(XMVector3Length(camera_->GetForwardVector()));
  auto radian_target_up = acosf(cos_target_up);
  if (radian_target_up > XM_PIDIV2)
    radian_target_up = XM_PI - radian_target_up;
  if (radian_target_up > XM_PIDIV4)
    radian_target_up = XM_PIDIV2 - radian_target_up;
  
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
  
  XMMATRIX matrix_rotate = XMMatrixRotationAxis(camera_->GetRightVector(), angle);

  // Change camera position:

  // Move target to origin
  auto pos_target = GetTargetPosition();
  auto vec = pos_target - XMVECTOR{ 0, 0, 0, 1.0f };
  auto position = GetPosition();
  position -= vec;

  // Rotate
  position = XMVector3Transform(position, matrix_rotate);
  
  // Move back
  position += vec;
  SetPosition(position);

  // Calulate new target vector.
  camera_->SetForwardVector(pos_target - position);
}

void EditorCameraController::Yaw(float angle) {
  XMMATRIX matrix_rotate = XMMatrixRotationAxis(XMVECTOR{ 0, 1.0f, 0 }, angle);

  // Change camera position:
  
  // Move target to origin
  auto pos_target = GetTargetPosition();
  auto vec = pos_target - XMVECTOR{ 0, 0, 0, 1.0f };
  auto position = GetPosition();
  position -= vec;
  
  // Rotate
  position = XMVector3Transform(position, matrix_rotate);
  
  // Move back
  position += vec;
  SetPosition(position);

  // Calulate new target vector:
  camera_->SetForwardVector(pos_target - position);
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

XMVECTOR EditorCameraController::GetTargetPosition() const {
  // Question:
  // pos_v_target = {0, 0, z}, calculate z.
  
  // Answer:
  // vec_v = pos_v_target_ - pos_v_origin, with pos_w_origin = {0, 0, 0}
  // vec_v * vec_v_up = 0, with vec_w_up = {0, 1.0f}

  // Program Answer:
  // vec_v = pos_v_target_ - pos_v_origin
  XMVECTOR pos_v_orgin = camera_->WorldToViewPosition(XMVECTOR{});
  float x_vec_v = -XMVectorGetX(pos_v_orgin);
  float y_vec_v = -XMVectorGetY(pos_v_orgin);
  // float z_vec_v = z_pos_v_target - XMVectorGetZ(pos_v_orgin);
  
  // vec_v * vec_v_up = 0
  // Expand,
  // x_vec_v * XMVectorGetX(vec_v_up) + y_vec_v * XMVectorGetY(vec_v_up) + z_vec_v * XMVectorGetZ(vec_v_up) = 0
  // So
  XMVECTOR vec_v_up = camera_->WorldToViewPosition(XMVECTOR{ 0, 1.0f });
  float z_vec_v = -(x_vec_v * XMVectorGetX(vec_v_up) + y_vec_v * XMVectorGetY(vec_v_up)) / XMVectorGetZ(vec_v_up);
  
  // So
  float z_pos_v_target = z_vec_v + XMVectorGetZ(pos_v_orgin);

  XMVECTOR pos_v_target{ 0, 0, z_pos_v_target, 1.0f };
  
  return camera_->ViewToWorldPosition(pos_v_target);
}

DirectX::XMVECTOR EditorCameraController::GetPosition() const
{
  return GetGameObject()->GetComponent<Transform>()->GetWorldPosition();
}

void EditorCameraController::SetPosition(Math::Vector3 value)
{
  GetGameObject()->GetComponent<Transform>()->SetWorldPosition(value);
}
