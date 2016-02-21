#pragma once

#include <Windows.h>
#include "Application.h"
#include "EditorCamera.h"
#include "FirstPersonalCamera.h"
#include "Engine.h"
#include "Stopwatch.h"

using namespace LL3D;
using namespace DirectX;

class Editor: public Application {
public:
  Editor();

  // fps <= 0 means unlimited.
  void SetMaxFPS(int fps);

protected:
  virtual void Update();

  // Handle mouse inputs:
  virtual void OnMouseDown(const MouseButtonEvent& event);
  virtual void OnMouseUp(const MouseButtonEvent& event);
  virtual void OnMouseMove(const MouseButtonEvent& event);
  virtual void OnMouseScroll(const MouseScrollEvent& event);

  virtual void OnResize();

private:
  void DoUpdate();

  EditorCamera editor_camera_;
  FirstPersonalCamera first_personal_camera_;
  Engine engine_;
  Stopwatch timer_;
  Lights lights_;
  
  int td_before_last_draw_ = 0;
  int max_fps_ = 0;
  IntPoint2 last_mouse_position_;
};
