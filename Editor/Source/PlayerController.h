#pragma once

#include <Behaviour.h>
#include <Input\Keyboard.h>
#include <Math\Math.h>
#include <GameObject.h>
#include <Transform.h>

class PlayerController : public LL3D::Behaviour {
public:
  PlayerController(LL3D::GameObject* object) : Behaviour(object) {}

  void Update() override {
    auto h = LL3D::Input::Keyboard::GetAxis(LL3D::Input::Keyboard::Horizontal);
    auto v = LL3D::Input::Keyboard::GetAxis(LL3D::Input::Keyboard::Vertical);
    auto p = object()->GetComponent<LL3D::Transform>()->position();
    p.x += h;
    p.z += v;
    object()->GetComponent<LL3D::Transform>()->SetPosition(p);
  }
};