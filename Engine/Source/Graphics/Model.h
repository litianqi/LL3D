#pragma once

#include <vector>
#include <filesystem>
#include <exception>
#include "../Math/Math.h"
#include "Material.h"

namespace LL3D {
namespace Graphics {

struct Vertex {
  Math::Vector3 position;
  Math::Vector3 normal;
  Math::Vector2 texcoord;
  Math::Vector3 tangent;  // todo
  Math::Vector3 bitangent;  // todo
};

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

  //>
  // Load a Model from file using Assimp.
  // Throw std::exception if filename doesn't exists or parsing faild. 
  //
  static Model LoadAssimp(std::experimental::filesystem::path pathname);
};

}  // namespace Graphics
}  // namespace LL3D