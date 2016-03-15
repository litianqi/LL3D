#pragma once

#include <Windows.h>
#include "Game.h"
#include "EditorCamera.h"
#include "Graphics/FirstPersonalCamera.h"
#include "Utils/Stopwatch.h"

using namespace DirectX;

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
