#pragma once

#include <chrono>
#include "Window.h"
#include "Graphics/GraphicsDevice.h"
#include "Assets.h"

namespace LL3D {

class Game : public Window {
public:
  Game();
  void Run();

protected:
  virtual void Update() = 0;

  std::unique_ptr<Graphics::GraphicsDevice> graphics_device_;
};

}  // namespace LL3D