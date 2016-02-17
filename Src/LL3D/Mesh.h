#pragma once

#include <vector>
#include <DirectXMath.h>
#include "Vertex.h"

namespace LL3D {

struct MeshData {
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;

  MeshData operator+(const MeshData& rhs) const;
  MeshData& operator+=(const MeshData& rhs) const;
};

// Creates a box centered at the origin with the given dimensions.
MeshData CreateBox(float width, float height, float depth, DirectX::FXMVECTOR color);

// Creates a sphere centered at the origin with the given radius.  The
// slices and stacks parameters control the degree of tessellation.
MeshData CreateSphere(float radius, int sliceCount, int stackCount, DirectX::FXMVECTOR color);

// Creates an mxn grid in the xz-plane with m rows and n columns, centered
// at the origin with the specified width and depth.
MeshData CreateGrid(float width, float depth, int m, int n, DirectX::FXMVECTOR color);

}
