#pragma once

#include <Behaviour.h>
#include <Input\Keyboard.h>
#include <Math\Math.h>
#include <Graphics\Model.h>
#include <GameObject.h>

class WaveController : public LL3D::Behaviour {
public:
  WaveController(LL3D::GameObject* object) : Behaviour(object) {}

  void Update() override {
    /*auto p = GetGameObject()->GetComponent<LL3D::Graphics::Model>()->GetTextureTransform();
    auto offset = LL3D::Math::Matrix::CreateTranslation(0.005f, 0, 0);
    GetGameObject()->GetComponent<LL3D::Graphics::Model>()->SetTextureTransform(p * offset);*/
  }
};