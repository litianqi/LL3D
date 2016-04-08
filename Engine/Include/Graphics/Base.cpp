#include "Graphics/Base.h"
#include "Core/Exceptions.h"
#include "Graphics/Device.h"
#include "Graphics/Effects.h"
#include "Graphics/Model.h"
#include "Graphics/CommonStates.h"
#include "Graphics/VertexTypes.h"

namespace LL3D {
namespace Graphics {

Device*                       Base::s_graphics_device;
std::unique_ptr<BasicEffect>  Base::s_effect;
Microsoft::WRL::ComPtr<ID3D11InputLayout>  Base::s_input_layout;

void Base::Initialize(Device* device) {
  s_graphics_device = device;
  
  // Init Effect.
  // todo: remove hard code.
  s_effect.reset(new BasicEffect("../Engine/_Resource/Effects/Main.fxo"));

  // Init InputLayout.
  const void* shader_bytecode;
  size_t shader_bytecode_size;
  s_effect->GetVertexShaderBytecode(&shader_bytecode, &shader_bytecode_size);
  ThrowIfFailed(
    s_graphics_device->GetDevice()->CreateInputLayout(Vertex::InputElements, 
      Vertex::InputElementCount, shader_bytecode, shader_bytecode_size, 
      &s_input_layout)
    );

  CommonStates::Initialize(device->GetDevice());
}

}  // namespace Graphics
}  // namespace LL3D
