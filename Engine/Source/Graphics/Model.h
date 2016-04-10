#pragma once

#include <vector>
#include <filesystem>
#include <exception>
#include "Math/Math.h"
#include "Material.h"
#include "VertexTypes.h"

namespace LL3D {
namespace Graphics {

//-----------------------------------------------------------------------------
// A mesh represents a geometry or model with a single material.
struct Mesh {
  std::vector<Vertex>       vertices;
  std::vector<unsigned int> indices;
  int                       materialIndex = -1;
  
  static Mesh createCube(float width, float height, float depth);
  static Mesh createSphere(float radius, int sliceCount, int stackCount);
  static Mesh createGrid(float width, float depth, int m, int n);
};

struct Model {
  std::string           name;
  std::vector<Mesh>     meshes;
  std::vector<Material> materials;
  DirectX::BoundingBox  boundingBox;

  static DirectX::BoundingBox localBoundingBox(const std::vector<Mesh>& meshs);
  //>
  // Load a Model from file using Assimp.
  // Throw std::exception if filename doesn't exists or parsing faild. 
  //
  static Model loadAssimp(std::experimental::filesystem::path pathname);
};

}  // namespace Graphics
}  // namespace LL3D