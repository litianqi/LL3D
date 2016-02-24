#pragma once

#include <Windows.h>
#include "Application.h"
#include "EditorCamera.h"
#include "FirstPersonalCamera.h"
#include "Engine.h"
#include "Stopwatch.h"

using namespace DirectX;

class Editor: public LL3D::Game {
public:
  Editor();

  // fps <= 0 means unlimited.
  void SetMaxFPS(int fps);

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
  void DoUpdate();

  EditorCamera editor_camera_;
  LL3D::FirstPersonalCamera first_personal_camera_;
  LL3D::Engine engine_;
  LL3D::Stopwatch timer_;
  LL3D::Lights lights_;
  
  int td_before_last_draw_ = 0;
  int max_fps_ = 0;
  LL3D::IntPoint2 last_mouse_position_;
};
