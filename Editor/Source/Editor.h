#pragma once

#include <Windows.h>
#include "Game.h"
#include "EditorCamera.h"
#include "FirstPersonalCamera.h"
#include "Engine.h"
#include "Core/Stopwatch.h"

using namespace DirectX;

class Editor: public LL3D::Game {
public:
  Editor();

protected:
  void Update() override;

 void OnMouseDown(const LL3D::MouseButtonEvent& event) override;
 void OnMouseUp(const LL3D::MouseButtonEvent& event) override;
 void OnMouseMove(const LL3D::MouseButtonEvent& event) override;
 void OnMouseScroll(const LL3D::MouseScrollEvent& event) override;
 void OnResize() override;

private:
  EditorCamera editor_camera_;
  LL3D::FirstPersonalCamera first_personal_camera_;
  LL3D::Engine engine_;
  LL3D::Stopwatch timer_;
  LL3D::Lights lights_;
  
  LL3D::IntPoint2 last_mouse_position_;
};
