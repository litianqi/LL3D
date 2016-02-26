#pragma once

#include <Windows.h>
#include "Game.h"
#include "EditorCamera.h"
#include "FirstPersonalCamera.h"
#include "Engine.h"
#include "Stopwatch.h"

using namespace DirectX;

class Editor: public LL3D::Game {
public:
  Editor();

protected:
  virtual void Update() override;
  virtual void Render() override;

  // Handle mouse inputs:
  virtual void OnMouseDown(const LL3D::MouseButtonEvent& event);
  virtual void OnMouseUp(const LL3D::MouseButtonEvent& event);
  virtual void OnMouseMove(const LL3D::MouseButtonEvent& event);
  virtual void OnMouseScroll(const LL3D::MouseScrollEvent& event);

  virtual void OnResize();

private:
  EditorCamera editor_camera_;
  LL3D::FirstPersonalCamera first_personal_camera_;
  LL3D::Engine engine_;
  LL3D::Stopwatch timer_;
  LL3D::Lights lights_;
  
  LL3D::IntPoint2 last_mouse_position_;
};
