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
  int                       material_index = -1;
  
  static Mesh CreateCube(float width, float height, float depth);
  static Mesh CreateSphere(float radius, int slice_count, int stack_count);
  static Mesh CreateGrid(float width, float depth, int m, int n);
};

struct Model {
  std::string           name;
  std::vector<Mesh>     meshes;
  std::vector<Material> materials;
  DirectX::BoundingBox  bounding_box;

  static DirectX::BoundingBox CalculateLocalBox(const std::vector<Mesh>& meshs);
  //>
  // Load a Model from file using Assimp.
  // Throw std::exception if filename doesn't exists or parsing faild. 
  //
  static Model LoadAssimp(std::experimental::filesystem::path pathname);
};

}  // namespace Graphics
}  // namespace LL3D