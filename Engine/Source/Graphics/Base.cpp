#include "Graphics/Base.h"
#include "Core/Exceptions.h"
#include "Graphics/Device.h"
#include "Graphics/Effects.h"
#include "Graphics/Model.h"
#include "Graphics/CommonStates.h"
#include "Graphics/VertexTypes.h"

namespace LL3D {
namespace Graphics {

Device*                       Base::s_graphicsDevice;
std::unique_ptr<BasicEffect>  Base::s_effect;
Microsoft::WRL::ComPtr<ID3D11InputLayout>  Base::s_inputLayout;

void Base::initialize(Device* device) {
  s_graphicsDevice = device;
  
  // Init Effect.
  // todo: remove hard code.
  s_effect.reset(new BasicEffect("../Engine/_Resource/Effects/Main.fxo"));

  // Init InputLayout.
  const void* shaderBytecode;
  size_t shaderBytecodeSize;
  s_effect->vertexShaderBytecode(&shaderBytecode, &shaderBytecodeSize);
  throwIfFailed(
    s_graphicsDevice->device()->CreateInputLayout(Vertex::kInputElements, 
      Vertex::kInputElementCount, shaderBytecode, shaderBytecodeSize, 
      &s_inputLayout)
    );

  CommonStates::initialize(device->device());
}

}  // namespace Graphics
}  // namespace LL3D
