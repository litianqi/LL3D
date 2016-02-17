#include "EditorCamera.h"
#include "Debug.h"

using namespace DirectX;

namespace LL3D {

EditorCamera::EditorCamera(Frustum frustum, DirectX::XMVECTOR pos,
  DirectX::XMVECTOR vec_target, DirectX::XMVECTOR vec_up) :
  Camera(frustum, pos, vec_target, vec_up) {
}

void EditorCamera::MoveLeftRight(float d) {
  pos_ += d * XMVector3Normalize(GetRightVector());
}

void EditorCamera::MoveUpDown(float d) {
  pos_ += d * XMVector3Normalize(vec_up_);
}

void EditorCamera::MoveBackForeward(float d) {
  pos_ += d * XMVector3Normalize(vec_target_);
}

void EditorCamera::Pitch(float radians) {
  DirectX::XMMATRIX matrix_rotate = DirectX::XMMatrixRotationAxis(GetRightVector(), radians);

  // Calculate new position.
  auto pos_target = GetTargetPos();
  pos_ -= pos_target;
  pos_ = XMVector3TransformNormal(pos_, matrix_rotate);
  pos_ += pos_target;

  // Calulate new target vector.
  vec_target_ = pos_target - pos_;

  // Calulate new up vector.
  vec_up_ -= pos_target;
  vec_up_ = XMVector3TransformNormal(vec_up_, matrix_rotate);
  vec_up_ += pos_target;

  Assert(!XMVector3Equal(vec_target_, XMVectorZero()));
  Assert(!XMVector3IsInfinite(vec_target_));
  Assert(!XMVector3Equal(vec_up_, XMVectorZero()));
  Assert(!XMVector3IsInfinite(vec_up_));
}

void EditorCamera::Yaw(float radians) {
  DirectX::XMMATRIX matrix_rotate = DirectX::XMMatrixRotationAxis(XMVECTOR{ 0, 1.0f, 0 }, radians);

  // Change camera position. 
  auto pos_target = GetTargetPos();
  pos_ -= pos_target;
  pos_ = XMVector3TransformNormal(pos_, matrix_rotate);
  pos_ += pos_target;

  // Calulate new target vector.
  vec_target_ = pos_target - pos_;

  // Calulate new up vector.
  vec_up_ -= pos_target;
  vec_up_ = XMVector3TransformNormal(vec_up_, matrix_rotate);
  vec_up_ -= pos_target;

  Assert(!XMVector3Equal(vec_target_, XMVectorZero()));
  Assert(!XMVector3IsInfinite(vec_target_));
  Assert(!XMVector3Equal(vec_up_, XMVectorZero()));
  Assert(!XMVector3IsInfinite(vec_up_));
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

  XMVECTOR pos_v_target{ 0, 0, z_pos_v_target };
  
  return ViewToWorldPos(pos_v_target);
}

}  // namespace LL3D
