#pragma once

#include <memory>
#include <wrl.h>

struct ID3D11InputLayout;

namespace LL3D {

class Game;

namespace Graphics {

class Device;
class BasicEffect;

class Base
{
public:
  static void initialize(Device* device);

protected:
  static Device* s_graphicsDevice;
  static std::unique_ptr<BasicEffect> s_effect;
  static Microsoft::WRL::ComPtr<ID3D11InputLayout> s_inputLayout;
};

} // namespace Graphics
} // namespace LL3D