#pragma once

#include <Behaviour.h>
#include <Input\Keyboard.h>
#include <Math\Math.h>
#include <GameObject.h>
#include <Transform.h>

class PlayerController : public LL3D::Behaviour {
public:
  PlayerController(LL3D::GameObject* object) : Behaviour(object) {}

  void update() override {
    auto h = LL3D::Input::Keyboard::axis(LL3D::Input::Keyboard::Horizontal);
    auto v = LL3D::Input::Keyboard::axis(LL3D::Input::Keyboard::Vertical);
    auto p = object()->component<LL3D::Transform>()->position();
    p.x += h;
    p.z += v;
    object()->component<LL3D::Transform>()->setPosition(p);
  }
};