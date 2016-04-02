#pragma once

#include <Game.h>
#include <Utils/Timer.h>
#include "EditorCameraController.h"
#include "FirstPersonalCamera.h"

class Editor: public LL3D::Game {
public:
  Editor();

protected:
  void Update() override;

  void OnResize() override;

private:
  //LL3D::Graphics::FirstPersonalCamera first_personal_camera_;
  LL3D::Utils::Stopwatch timer_;  
};
