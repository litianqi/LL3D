#include "Base.h"
#include "Device.h"
#include "Effects.h"
#include "Model.h"

namespace LL3D {
namespace Graphics {

Device*                       Base::s_graphics_device;
std::unique_ptr<BasicEffect>  Base::s_effect;
std::unique_ptr<InputLayout>  Base::s_input_layout;

void Base::Init(Device* device) {
  s_graphics_device = device;
  // TODO: remove hard code.
  s_effect.reset(new BasicEffect("../Engine/Resource/Shaders/Main.fxo"));
  s_input_layout.reset(new InputLayout);
}

}  // namespace Graphics
}  // namespace LL3D
