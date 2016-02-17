#pragma once

#include <boost\noncopyable.hpp>
#include <Windows.h>
#include "Engine.h"
#include "Window.h"
#include "Stopwatch.h"

namespace LL3D {

class Application: public Window {
public:
  Application();

  void Run(); 
  // fps <= 0 means unlimited.
  void SetMaxFPS(int fps);

  // Handle mouse inputs:
  virtual void OnMouseDown(const MouseButtonEvent& event);
  virtual void OnMouseUp(const MouseButtonEvent& event);
  virtual void OnMouseMove(const MouseButtonEvent& event);
  virtual void OnMouseScroll(const MouseScrollEvent& event);

  virtual void OnResize();

protected:
  Engine engine_;
  Stopwatch timer_;
  int td_before_last_draw_ = 0;
  int max_fps_ = 0;
};

}  // namespace LL3D