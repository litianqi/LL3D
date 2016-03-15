#pragma once

#include <vector>
#include <filesystem>
#include <wrl.h>
#include <DirectXMath.h>
#include "../Component.h"
#include "../Core/Uncopyable.h"
#include "Base.h"
#include "Material.h"

struct ID3D11InputLayout;
struct ID3D11Buffer;

namespace LL3D {
namespace Graphics {

class Effect;
class BasicEffect;

struct Vertex {
  DirectX::XMVECTOR position;
  DirectX::XMVECTOR normal;
  DirectX::XMFLOAT2 texture_coordinate;
};

class InputLayout : private Base, private Uncopyable {
public:
  InputLayout();

  operator ID3D11InputLayout*();

private:
  Microsoft::WRL::ComPtr<ID3D11InputLayout> layout_;
};

class Model : public Component, private Base {
public:
  struct Mesh {
    std::vector<Vertex>       vertices;
    std::vector<unsigned int> indices;
  };

  Model(const Mesh& mesh, const Material& material, 
    const std::string& texture_path, DirectX::FXMMATRIX texture_transform);
  std::unique_ptr<Component> Clone() override;

  void Update() override;

private:
  std::string       id_;
  Mesh              mesh_;
  Material          material_;
  std::string       texture_path_;
  DirectX::XMMATRIX texture_transform_;

  Microsoft::WRL::ComPtr<ID3D11Buffer>  index_buffer_;
  Microsoft::WRL::ComPtr<ID3D11Buffer>  vertex_buffer_;
};

// Creates a box centered at the origin with the given dimensions.
Model::Mesh CreateBox(float width, float height, float depth);

// Creates a sphere centered at the origin with the given radius.  The
// slices and stacks parameters control the degree of tessellation.
Model::Mesh CreateSphere(float radius, int sliceCount, int stackCount);

// Creates an mxn grid in the xz-plane with m rows and n columns, centered
// at the origin with the specified width and depth.
Model::Mesh CreateGrid(float width, float depth, int m, int n);

}  // namespace Graphics
}  // namespace LL3D
