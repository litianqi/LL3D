#pragma once

#include <memory>
#include <wrl.h>

struct ID3D11InputLayout;

namespace LL3D {

class Game;

namespace Graphics {

class Device;
class BasicEffect;

class Base {
public:
  static void Initialize(Device* device);

protected:
  static Device*                      s_graphics_device;
  static std::unique_ptr<BasicEffect> s_effect;
  static Microsoft::WRL::ComPtr<ID3D11InputLayout> s_input_layout;
};

}  // namespace Graphics
}  // namespace LL3D