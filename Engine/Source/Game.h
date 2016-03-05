#pragma once

#include <chrono>
#include "Window.h"
#include "Graphics/GraphicsDevice.h"
#include "Assets.h"

namespace LL3D {

class Game {
public:
  Game();

  void Run();

protected:
  virtual void Update() = 0;
  virtual void OnMouseDown(const MouseButtonEvent& event) = 0;
  virtual void OnMouseUp(const MouseButtonEvent& event) = 0;
  virtual void OnMouseMove(const MouseButtonEvent& event) = 0;
  virtual void OnMouseScroll(const MouseScrollEvent& event) = 0;
  virtual void OnResize() = 0;

  std::unique_ptr<Window>                   window_;
  std::unique_ptr<Graphics::GraphicsDevice> graphics_device_;
};

}  // namespace LL3D