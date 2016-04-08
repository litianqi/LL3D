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

  void Run();

protected:
  virtual void Update() = 0;
  virtual void OnResize() = 0;

  std::unique_ptr<Window>           window_;
  std::unique_ptr<Graphics::Device> graphics_device_;
  std::unique_ptr<Scene>            scene_;
};

}  // namespace LL3D