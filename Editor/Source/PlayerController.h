#pragma once

#include <Input/Mouse.h>
#include <Input/Keyboard.h>
#include <Behaviour.h>
#include <GameObject.h>
#include <Transform.h>

// Fisrt personal camera controller.
class PlayerController : public LL3D::Behaviour {
public:
  void start() override
  {
    transform_ = &(object()->transform());
    last_mouse_position_ = LL3D::Input::Mouse::position();
  }

  // Moves or rotates camera based input in last frame.
  void update() override
  {
    using namespace LL3D;

    // Move.
    const auto h = LL3D::Input::Keyboard::axis(LL3D::Input::Keyboard::Horizontal);
    const auto v = LL3D::Input::Keyboard::axis(LL3D::Input::Keyboard::Vertical);
    moveRight(h);
    moveForeward(v);
    scrollNear(Input::Mouse::scrollDelta() * 0.02f);

    // Rotate.
    const auto delta = Input::Mouse::position() - last_mouse_position_;
    if (Input::Mouse::isHeldingDown(Input::Mouse::kRight)) {
      yawFP(0.005f * delta.x);
      pitchFP(0.005f * delta.y);
    }
    if (Input::Mouse::isHeldingDown(Input::Mouse::kLeft)) {
      yawSP(0.005f * delta.x);
      pitchSP(0.005f * delta.y);
    }

    last_mouse_position_ = Input::Mouse::position();
  }

  void moveForeward(float d)
  {
    const auto new_pos = transform_->position() + transform_->forwardVec() * d;
    transform_->setPosition(new_pos);
  }

  void moveUp(float d)
  {
    const auto new_pos = transform_->position() + transform_->upVec() * d;
    transform_->setPosition(new_pos);
  }

  void moveRight(float d)
  {
    const auto new_pos = transform_->position() + transform_->rightVec() * d;
    transform_->setPosition(new_pos);
  }

  void scrollNear(float d)
  {

  }

  // Pitch (rotates around x axis) from first personal perspective.
  void pitchFP(float angle)
  {
    const auto new_rotation = transform_->rotation() + LL3D::Math::Vector3(angle, 0.f, 0.f);
    transform_->setRotation(new_rotation);
  }

  // Yaw (rotates around y axis) from first personal perspective.
  void yawFP(float angle)
  {
    const auto new_rotation = transform_->rotation() + LL3D::Math::Vector3(0.f, angle, 0.f);
    transform_->setRotation(new_rotation);
  }

  // Pitch (rotates around x axis) from second personal perspective.
  void pitchSP(float angle)
  {

  }

  // Yaw (rotates around y axis) from second personal perspective.
  void yawSP(float angle)
  {

  }

private:
  LL3D::Transform* transform_;
  LL3D::Math::Vector2 last_mouse_position_;
};
