#pragma once

namespace LL3D {

class GameObject;

namespace Graphics {

/**
 * Objects uses Camera must inherits ConstCameraUser or CameraUser.
 * Then when camera changes, all camera users will be notified.
 */
class ConstCameraUser
{
public:
  ConstCameraUser(const GameObject* camera)
    : camera_(camera)
  {
  }
  void setCamera(const GameObject* camera) { camera_ = camera; }
protected:
  const GameObject* camera_;
};

/**
* Objects uses Camera must inherits ConstCameraUser or CameraUser.
* Then when camera changes, all camera users will be notified.
*/
class CameraUser
{
public:
  CameraUser(GameObject* camera)
    : camera_(camera)
  {
  }
  void setCamera(GameObject* camera) { camera_ = camera; }
protected:
  GameObject* camera_;
};

} // namespace Graphics
} // namespace LL3D