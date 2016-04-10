#pragma once

#include <chrono>
#include <memory>

namespace LL3D {

class Window;
class Scene;
namespace Graphics {
class Device;
}

class Game {
public:
  Game();

  void run();

protected:
  virtual void update() = 0;
  virtual void onResize() = 0;

  std::unique_ptr<Window>           window_;
  std::unique_ptr<Graphics::Device> graphics_device_;
  std::unique_ptr<Scene>            scene_;
};

}  // namespace LL3D