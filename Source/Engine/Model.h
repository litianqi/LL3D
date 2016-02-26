#pragma once

#include <vector>
#include <DirectXMath.h>
#include "Material.h"

struct ID3D11Device;
struct ID3D11InputLayout;

namespace LL3D {

using namespace DirectX;

class BasicEffect;

struct Vertex {

  class InputLayout {
  public:
    InputLayout(ID3D11Device* device, BasicEffect& effect);
    operator ID3D11InputLayout*();

  private:
    ID3D11InputLayout* layout_;
  };

  XMVECTOR position;
  XMVECTOR normal;
};

struct Model {

  struct Mesh {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
  };
  
  Mesh mesh;
  XMMATRIX world;
  Material material;
};

// Helper function for DirectX to combine multiple meshs.
Model::Mesh CombineMeshes(const Model::Mesh& lhs, const Model::Mesh& rhs);
Model::Mesh CombineMeshes(const std::vector<Model>& meshs);

// Creates a box centered at the origin with the given dimensions.
Model::Mesh CreateBox(float width, float height, float depth);

// Creates a sphere centered at the origin with the given radius.  The
// slices and stacks parameters control the degree of tessellation.
Model::Mesh CreateSphere(float radius, int sliceCount, int stackCount);

// Creates an mxn grid in the xz-plane with m rows and n columns, centered
// at the origin with the specified width and depth.
Model::Mesh CreateGrid(float width, float depth, int m, int n);

}  // namespace LL3D
