#include "EditorCamera.h"
#include "Core/Assert.h"

EditorCamera::EditorCamera(Frustum frustum, XMVECTOR position, XMVECTOR forward_vector) :
  Camera(frustum, position, forward_vector) {
}

void EditorCamera::MoveLeftRight(float d) {
  position_ += d * XMVector3Normalize(GetRightVector());
}

void EditorCamera::MoveUpDown(float d) {
  position_ += d * XMVector3Normalize(GetUpVector());
}

void EditorCamera::MoveBackForeward(float d) {
  position_ += d * XMVector3Normalize(forward_vector_);
}

void EditorCamera::Pitch(float angle) {
  // Prevent target vector too close to (or even pass) up vector:
  
  // Get angle between target and up.
  auto cos_target_up = XMVectorGetX(XMVector3Dot(forward_vector_, XMVECTOR{ 0, 1.0f })) / XMVectorGetX(XMVector3Length(forward_vector_));
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
  
  XMMATRIX matrix_rotate = XMMatrixRotationAxis(GetRightVector(), angle);

  // Change camera position:

  // Move target to origin
  auto pos_target = GetTargetPosition();
  auto vec = pos_target - XMVECTOR{ 0, 0, 0, 1.0f };
  position_ -= vec;

  // Rotate
  position_ = XMVector3Transform(position_, matrix_rotate);
  
  // Move back
  position_ += vec;

  // Calulate new target vector.
  forward_vector_ = pos_target - position_;
}

void EditorCamera::Yaw(float angle) {
  XMMATRIX matrix_rotate = XMMatrixRotationAxis(XMVECTOR{ 0, 1.0f, 0 }, angle);

  // Change camera position:
  
  // Move target to origin
  auto pos_target = GetTargetPosition();
  auto vec = pos_target - XMVECTOR{ 0, 0, 0, 1.0f };
  position_ -= vec;
  
  // Rotate
  position_ = XMVector3Transform(position_, matrix_rotate);
  
  // Move back
  position_ += vec;

  // Calulate new target vector:
  forward_vector_ = pos_target - position_;
}

XMVECTOR EditorCamera::GetTargetPosition() const {
  // Question:
  // pos_v_target = {0, 0, z}, calculate z.
  
  // Answer:
  // vec_v = pos_v_target_ - pos_v_origin, with pos_w_origin = {0, 0, 0}
  // vec_v * vec_v_up = 0, with vec_w_up = {0, 1.0f}

  // Program Answer:
  // vec_v = pos_v_target_ - pos_v_origin
  XMVECTOR pos_v_orgin = WorldToViewPosition(XMVECTOR{});
  float x_vec_v = -XMVectorGetX(pos_v_orgin);
  float y_vec_v = -XMVectorGetY(pos_v_orgin);
  // float z_vec_v = z_pos_v_target - XMVectorGetZ(pos_v_orgin);
  
  // vec_v * vec_v_up = 0
  // Expand,
  // x_vec_v * XMVectorGetX(vec_v_up) + y_vec_v * XMVectorGetY(vec_v_up) + z_vec_v * XMVectorGetZ(vec_v_up) = 0
  // So
  XMVECTOR vec_v_up = WorldToViewPosition(XMVECTOR{ 0, 1.0f });
  float z_vec_v = -(x_vec_v * XMVectorGetX(vec_v_up) + y_vec_v * XMVectorGetY(vec_v_up)) / XMVectorGetZ(vec_v_up);
  
  // So
  float z_pos_v_target = z_vec_v + XMVectorGetZ(pos_v_orgin);

  XMVECTOR pos_v_target{ 0, 0, z_pos_v_target, 1.0f };
  
  return ViewToWorldPosition(pos_v_target);
}