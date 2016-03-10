#pragma once

#include <memory>
#include <wrl.h>

namespace LL3D {

class Game;

namespace Graphics {

class Device;
class BasicEffect;
class InputLayout;

class Base {
  friend Game;

protected:
  static Device*                      s_graphics_device;
  static std::unique_ptr<BasicEffect> s_effect;
  static std::unique_ptr<InputLayout> s_input_layout;

private:
  static void Init(Device* device);
};

}  // namespace Graphics
}  // namespace LL3D