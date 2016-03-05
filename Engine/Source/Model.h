#pragma once

#include <vector>
#include <filesystem>
#include <wrl.h>
#include <DirectXMath.h>
#include "Material.h"
#include "Core\Uncopyable.h"

struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11InputLayout;
struct ID3D11Buffer;

namespace LL3D {

using namespace DirectX;
using namespace std::experimental;

class Effect;
class BasicEffect;

struct Vertex {

  class InputLayout : private Uncopyable {
  public:
    InputLayout(ID3D11Device* device, Effect* effect);

    operator ID3D11InputLayout*();

  private:
    Microsoft::WRL::ComPtr<ID3D11InputLayout> layout_;
  };

  XMVECTOR position;
  XMVECTOR normal;
  XMFLOAT2 texture_coordinate;
};

class Model {
public:
  struct Mesh {
    std::vector<Vertex>       vertices;
    std::vector<unsigned int> indices;
  };

  Model(ID3D11Device* device, const std::string& id, const Mesh& mesh, DirectX::FXMMATRIX world, 
    const Material& material, const std::string& texture_path, DirectX::FXMMATRIX texture_transform);
  virtual ~Model() {}
  
  void Render(ID3D11DeviceContext* device_context, BasicEffect* effect, ID3D11InputLayout* input_layout);

private:
  std::string  id_;
  Mesh         mesh_;
  XMMATRIX     world_;
  Material     material_;
  std::string  texture_path_;
  XMMATRIX     texture_transform_;
  
  Microsoft::WRL::ComPtr<ID3D11Buffer>  index_buffer_;
  Microsoft::WRL::ComPtr<ID3D11Buffer>  vertex_buffer_;
  Microsoft::WRL::ComPtr<ID3D11Device>  device_;
};

// Creates a box centered at the origin with the given dimensions.
Model::Mesh CreateBox(float width, float height, float depth);

// Creates a sphere centered at the origin with the given radius.  The
// slices and stacks parameters control the degree of tessellation.
Model::Mesh CreateSphere(float radius, int sliceCount, int stackCount);

// Creates an mxn grid in the xz-plane with m rows and n columns, centered
// at the origin with the specified width and depth.
Model::Mesh CreateGrid(float width, float depth, int m, int n);

}  // namespace LL3D
