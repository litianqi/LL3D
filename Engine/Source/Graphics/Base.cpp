#include "Base.h"
#include "Device.h"
#include "Effects.h"
#include "Model.h"
#include "..\Core\Exceptions.h"

namespace LL3D {
namespace Graphics {

Device*                       Base::s_graphics_device;
std::unique_ptr<BasicEffect>  Base::s_effect;
Microsoft::WRL::ComPtr<ID3D11InputLayout>  Base::s_input_layout;

void Base::Init(Device* device) {
  s_graphics_device = device;
  
  // Init Effect.
  // todo: remove hard code.
  s_effect.reset(new BasicEffect("../Engine/Resource/Shaders/Main.fxo"));
  D3D11_INPUT_ELEMENT_DESC vertex_desc[] =
  {
    { "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT , D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT , D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT , D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "TANGENT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT , D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "BITANGENT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT , D3D11_INPUT_PER_VERTEX_DATA, 0 }
  };

  // Init InputLayout.
  const void* shader_bytecode;
  size_t shader_bytecode_size;
  s_effect->GetVertexShaderBytecode(&shader_bytecode, &shader_bytecode_size);
  ThrowIfFailed(
    s_graphics_device->GetDevice()->CreateInputLayout(vertex_desc, 5, 
      shader_bytecode, shader_bytecode_size, &s_input_layout)
    );
}

}  // namespace Graphics
}  // namespace LL3D
