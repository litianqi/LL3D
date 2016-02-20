#include "EditorCamera.h"
#include "Debug.h"

using namespace DirectX;

namespace LL3D {

EditorCamera::EditorCamera(Frustum frustum, DirectX::XMVECTOR pos, DirectX::XMVECTOR vec_target) :
  Camera(frustum, pos, vec_target) {
}

void EditorCamera::MoveLeftRight(float d) {
  pos_ += d * XMVector3Normalize(GetRightVector());
}

void EditorCamera::MoveUpDown(float d) {
  pos_ += d * XMVector3Normalize(GetUpVector());
}

void EditorCamera::MoveBackForeward(float d) {
  pos_ += d * XMVector3Normalize(vec_target_);
}

void EditorCamera::Pitch(float radians) {
  // Prevent target vector too close to (or even pass) up vector:
  
  // Get angle between target and up.
  auto cos_target_up = XMVectorGetX(XMVector3Dot(vec_target_, XMVECTOR{ 0, 1.0f })) / XMVectorGetX(XMVector3Length(vec_target_));
  auto radian_target_up = acosf(cos_target_up);
  if (radian_target_up > XM_PIDIV2)
    radian_target_up = XM_PI - radian_target_up;
  if (radian_target_up > XM_PIDIV4)
    radian_target_up = XM_PIDIV2 - radian_target_up;
  
  // Correct angle.
  if (cos_target_up < 0 &&  // Target vector towards Up vector.
    radians > 0) {
    if (radians >= radian_target_up) {
      radians = radian_target_up - 0.002f;
    }
  }
  else if (((cos_target_up > 0 &&  // Target vector leaves Up vector.
    radians < 0))) {
    if (std::fabs(radians) >= radian_target_up) {
      radians = -(radian_target_up - 0.002f);
    }
  }
  
  DirectX::XMMATRIX matrix_rotate = DirectX::XMMatrixRotationAxis(GetRightVector(), radians);

  // Change camera position:

  // Move target to origin
  auto pos_target = GetTargetPos();
  auto vec = pos_target - XMVECTOR{ 0, 0, 0, 1.0f };
  pos_ -= vec;

  // Rotate
  pos_ = XMVector3Transform(pos_, matrix_rotate);
  
  // Move back
  pos_ += vec;

  // Calulate new target vector.
  vec_target_ = pos_target - pos_;
}

void EditorCamera::Yaw(float radians) {
  DirectX::XMMATRIX matrix_rotate = DirectX::XMMatrixRotationAxis(XMVECTOR{ 0, 1.0f, 0 }, radians);

  // Change camera position:
  
  // Move target to origin
  auto pos_target = GetTargetPos();
  auto vec = pos_target - XMVECTOR{ 0, 0, 0, 1.0f };
  pos_ -= vec;
  
  // Rotate
  pos_ = XMVector3Transform(pos_, matrix_rotate);
  
  // Move back
  pos_ += vec;

  // Calulate new target vector:
  vec_target_ = pos_target - pos_;
}

DirectX::XMVECTOR EditorCamera::GetTargetPos() const {
  // Question:
  // pos_v_target = {0, 0, z}, calculate z.
  
  // Answer:
  // vec_v = pos_v_target_ - pos_v_origin, with pos_w_origin = {0, 0, 0}
  // vec_v * vec_v_up = 0, with vec_w_up = {0, 1.0f}

  // Program Answer:
  // vec_v = pos_v_target_ - pos_v_origin
  XMVECTOR pos_v_orgin = WorldToViewPos(XMVECTOR{});
  float x_vec_v = -XMVectorGetX(pos_v_orgin);
  float y_vec_v = -XMVectorGetY(pos_v_orgin);
  // float z_vec_v = z_pos_v_target - XMVectorGetZ(pos_v_orgin);
  
  // vec_v * vec_v_up = 0
  // Expand,
  // x_vec_v * XMVectorGetX(vec_v_up) + y_vec_v * XMVectorGetY(vec_v_up) + z_vec_v * XMVectorGetZ(vec_v_up) = 0
  // So
  XMVECTOR vec_v_up = WorldToViewPos(XMVECTOR{ 0, 1.0f });
  float z_vec_v = -(x_vec_v * XMVectorGetX(vec_v_up) + y_vec_v * XMVectorGetY(vec_v_up)) / XMVectorGetZ(vec_v_up);
  
  // So
  float z_pos_v_target = z_vec_v + XMVectorGetZ(pos_v_orgin);

  XMVECTOR pos_v_target{ 0, 0, z_pos_v_target, 1.0f };
  
  return ViewToWorldPos(pos_v_target);
}

}  // namespace LL3D
