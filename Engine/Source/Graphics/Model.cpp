#include "Model.h"
#include <DirectXMath.h>
#include <D3D11.h>
#include <d3dx11effect.h>
#include "../Core/Assert.h"
#include "../Core/Exceptions.h"
#include "Device.h"
#include "Color.h"
#include "Effects.h"
#include "../Assets.h"

using namespace DirectX;
using namespace std::experimental;

namespace LL3D {
namespace Graphics {

InputLayout::InputLayout() {
  D3D11_INPUT_ELEMENT_DESC vertex_desc[] =
  {
    { "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 }
  };

  const void* shader_bytecode;
  size_t shader_bytecode_size;
  s_effect->GetVertexShaderBytecode(&shader_bytecode, &shader_bytecode_size);
  ThrowIfFailed(
    s_graphics_device->GetDevice()->CreateInputLayout(vertex_desc, 3, shader_bytecode, shader_bytecode_size, &layout_)
    );
}

InputLayout::operator ID3D11InputLayout*() {
  return layout_.Get();
}


Model::Model(const Mesh& mesh, const Material& material,
  const std::string& texture_path, Math::Matrix texture_transform,
  bool enable_back_face_cull) :
  mesh_(mesh), material_(material), 
  texture_path_(texture_path), texture_transform_(texture_transform)
{
  // Creates vertext and index buffer.
  D3D11_BUFFER_DESC vbd;
  vbd.Usage = D3D11_USAGE_IMMUTABLE;
  vbd.ByteWidth = static_cast<UINT>(sizeof(Vertex) * mesh.vertices.size());
  vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
  vbd.CPUAccessFlags = 0;
  vbd.MiscFlags = 0;
  D3D11_SUBRESOURCE_DATA vinitData;
  vinitData.pSysMem = mesh.vertices.data();

  ThrowIfFailed(
    s_graphics_device->GetDevice()->CreateBuffer(&vbd, &vinitData, &vertex_buffer_)
    );

  D3D11_BUFFER_DESC ibd;
  ibd.Usage = D3D11_USAGE_IMMUTABLE;
  ibd.ByteWidth = static_cast<UINT>(sizeof(unsigned int) * mesh.indices.size());
  ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
  ibd.CPUAccessFlags = 0;
  ibd.MiscFlags = 0;
  D3D11_SUBRESOURCE_DATA iinitData;
  iinitData.pSysMem = mesh.indices.data();

  ThrowIfFailed(
    s_graphics_device->GetDevice()->CreateBuffer(&ibd, &iinitData, &index_buffer_)
    );

  if (!enable_back_face_cull) {
    auto no_cull_desc = D3D11_RASTERIZER_DESC();
    no_cull_desc.FillMode = D3D11_FILL_SOLID;
    no_cull_desc.CullMode = D3D11_CULL_NONE;
    no_cull_desc.FrontCounterClockwise = false;
    no_cull_desc.DepthClipEnable = true;

    ThrowIfFailed(
      s_graphics_device->GetDevice()->CreateRasterizerState(&no_cull_desc,
        &rasterizer_state_)
      );
  }
}

std::unique_ptr<Component> Model::Clone() {
  return std::unique_ptr<Component>(new Model(*this));
}

void Model::SetTextureTransform(const Math::Matrix & value) {
  texture_transform_ = value;
}

const Math::Matrix & Model::GetTextureTransform() const {
  return texture_transform_;
}

void Model::Update() {

  UINT stride = sizeof(Vertex);
  UINT offset = 0;
  s_graphics_device->GetDeviceContex()->IASetVertexBuffers(0, 1, vertex_buffer_.GetAddressOf(), &stride, &offset);
  s_graphics_device->GetDeviceContex()->IASetIndexBuffer(index_buffer_.Get(), DXGI_FORMAT_R32_UINT, 0);

  //for (UINT pass = 0; pass < effect->GetPassNum(); ++pass) {
  // Set per object constant buffer.
  s_effect->SetMaterial(material_);
  s_effect->SetTextureTransform(texture_transform_);
  s_effect->SetTexture(CreateTexture(s_graphics_device->GetDevice(), texture_path_));

  // Apply rasterizer option, if specified.
  s_graphics_device->GetDeviceContex()->RSSetState(rasterizer_state_.Get());

  // Draw object.
  s_effect->Apply(s_graphics_device->GetDeviceContex());
  s_graphics_device->GetDeviceContex()->DrawIndexed(
    static_cast<UINT>(mesh_.indices.size()), 0, 0);
  
  // Roll back rasterizer option. For it's global so will affect other models.
  s_graphics_device->GetDeviceContex()->RSSetState(0);
}

Model::Mesh CreateBox(float width, float height, float depth) {
  Model::Mesh mesh;

  //
  // Create the vertices.
  //

  float w2 = 0.5f*width;
  float h2 = 0.5f*height;
  float d2 = 0.5f*depth;

  // Create the back face vertex data.
  mesh.vertices.push_back(Vertex{ XMVECTOR{ -w2, -h2, -d2, 1.0f }, XMVECTOR{ 0, 0, -1.0f }, XMFLOAT2{ 1, 1 } });
  mesh.vertices.push_back(Vertex{ XMVECTOR{ -w2, +h2, -d2, 1.0f }, XMVECTOR{ 0, 0, -1.0f }, XMFLOAT2{ 0, 1 } });
  mesh.vertices.push_back(Vertex{ XMVECTOR{ +w2, +h2, -d2, 1.0f }, XMVECTOR{ 0, 0, -1.0f }, XMFLOAT2{ 0, 0 } });
  mesh.vertices.push_back(Vertex{ XMVECTOR{ +w2, -h2, -d2, 1.0f }, XMVECTOR{ 0, 0, -1.0f }, XMFLOAT2{ 1, 0 } });

  // Create the front face vertex data.
  mesh.vertices.push_back(Vertex{ XMVECTOR{ -w2, -h2, +d2, 1.0f }, XMVECTOR{ 0, 0, 1.0f }, XMFLOAT2{ 0, 1 } });
  mesh.vertices.push_back(Vertex{ XMVECTOR{ +w2, -h2, +d2, 1.0f }, XMVECTOR{ 0, 0, 1.0f }, XMFLOAT2{ 0, 0 } });
  mesh.vertices.push_back(Vertex{ XMVECTOR{ +w2, +h2, +d2, 1.0f }, XMVECTOR{ 0, 0, 1.0f }, XMFLOAT2{ 1, 0 } });
  mesh.vertices.push_back(Vertex{ XMVECTOR{ -w2, +h2, +d2, 1.0f }, XMVECTOR{ 0, 0, 1.0f }, XMFLOAT2{ 1, 1 } });

  // Create the top face vertex data.
  mesh.vertices.push_back(Vertex{ XMVECTOR{ -w2, +h2, -d2, 1.0f }, XMVECTOR{ 0, 1.0f, 0 }, XMFLOAT2{ 0, 1 } });
  mesh.vertices.push_back(Vertex{ XMVECTOR{ -w2, +h2, +d2, 1.0f }, XMVECTOR{ 0, 1.0f, 0 }, XMFLOAT2{ 0, 0 } });
  mesh.vertices.push_back(Vertex{ XMVECTOR{ +w2, +h2, +d2, 1.0f }, XMVECTOR{ 0, 1.0f, 0 }, XMFLOAT2{ 1, 0 } });
  mesh.vertices.push_back(Vertex{ XMVECTOR{ +w2, +h2, -d2, 1.0f }, XMVECTOR{ 0, 1.0f, 0 }, XMFLOAT2{ 1, 1 } });
                                                                        
  // Create the bottom face vertex data.                               
  mesh.vertices.push_back(Vertex{ XMVECTOR{ -w2, -h2, -d2, 1.0f }, XMVECTOR{ 0, -1.0f, 0 }, XMFLOAT2{ 0, 1 } });
  mesh.vertices.push_back(Vertex{ XMVECTOR{ +w2, -h2, -d2, 1.0f }, XMVECTOR{ 0, -1.0f, 0 }, XMFLOAT2{ 0, 0 } });
  mesh.vertices.push_back(Vertex{ XMVECTOR{ +w2, -h2, +d2, 1.0f }, XMVECTOR{ 0, -1.0f, 0 }, XMFLOAT2{ 1, 0 } });
  mesh.vertices.push_back(Vertex{ XMVECTOR{ -w2, -h2, +d2, 1.0f }, XMVECTOR{ 0, -1.0f, 0 }, XMFLOAT2{ 1, 1 } });
                                                                        
  // Create the left face vertex data.                                 
  mesh.vertices.push_back(Vertex{ XMVECTOR{ -w2, -h2, +d2, 1.0f }, XMVECTOR{ -1.0f, 0, 0 }, XMFLOAT2{ 0, 1 } });
  mesh.vertices.push_back(Vertex{ XMVECTOR{ -w2, +h2, +d2, 1.0f }, XMVECTOR{ -1.0f, 0, 0 }, XMFLOAT2{ 0, 0 } });
  mesh.vertices.push_back(Vertex{ XMVECTOR{ -w2, +h2, -d2, 1.0f }, XMVECTOR{ -1.0f, 0, 0 }, XMFLOAT2{ 1, 0 } });
  mesh.vertices.push_back(Vertex{ XMVECTOR{ -w2, -h2, -d2, 1.0f }, XMVECTOR{ -1.0f, 0, 0 }, XMFLOAT2{ 1, 1 } });
                                                                        
  // Create the right face vertex data.                                
  mesh.vertices.push_back(Vertex{ XMVECTOR{ +w2, -h2, -d2, 1.0f }, XMVECTOR{ 1.0f, 0, 0 }, XMFLOAT2{ 0, 1 } });
  mesh.vertices.push_back(Vertex{ XMVECTOR{ +w2, +h2, -d2, 1.0f }, XMVECTOR{ 1.0f, 0, 0 }, XMFLOAT2{ 0, 0 } });
  mesh.vertices.push_back(Vertex{ XMVECTOR{ +w2, +h2, +d2, 1.0f }, XMVECTOR{ 1.0f, 0, 0 }, XMFLOAT2{ 1, 0 } });
  mesh.vertices.push_back(Vertex{ XMVECTOR{ +w2, -h2, +d2, 1.0f }, XMVECTOR{ 1.0f, 0, 0 }, XMFLOAT2{ 1, 1 } });


  //
  // Create the indices.
  //

  mesh.indices.resize(36);

  // Fill in the front face index data
  mesh.indices[0] = 0; mesh.indices[1] = 1; mesh.indices[2] = 2;
  mesh.indices[3] = 0; mesh.indices[4] = 2; mesh.indices[5] = 3;

  // Fill in the back face index data
  mesh.indices[6] = 4; mesh.indices[7] = 5; mesh.indices[8] = 6;
  mesh.indices[9] = 4; mesh.indices[10] = 6; mesh.indices[11] = 7;

  // Fill in the top face index data
  mesh.indices[12] = 8; mesh.indices[13] = 9; mesh.indices[14] = 10;
  mesh.indices[15] = 8; mesh.indices[16] = 10; mesh.indices[17] = 11;

  // Fill in the bottom face index data
  mesh.indices[18] = 12; mesh.indices[19] = 13; mesh.indices[20] = 14;
  mesh.indices[21] = 12; mesh.indices[22] = 14; mesh.indices[23] = 15;

  // Fill in the left face index data
  mesh.indices[24] = 16; mesh.indices[25] = 17; mesh.indices[26] = 18;
  mesh.indices[27] = 16; mesh.indices[28] = 18; mesh.indices[29] = 19;

  // Fill in the right face index data
  mesh.indices[30] = 20; mesh.indices[31] = 21; mesh.indices[32] = 22;
  mesh.indices[33] = 20; mesh.indices[34] = 22; mesh.indices[35] = 23;

  return mesh;
}

Model::Mesh CreateSphere(float radius, int sliceCount, int stackCount) {
  Model::Mesh mesh;

  //
  // Compute the vertices stating at the top pole and moving down the stacks.
  //

  // Poles: note that there will be texture coordinate distortion as there is
  // not a unique point on the texture map to assign to the pole when mapping
  // a rectangular texture onto a sphere.
  Vertex topVertex{ XMVECTOR{ 0.0f, +radius, 0.0f, 1.0f }, XMVECTOR{ 0.0f, 1.0f, 0.0f } };
  Vertex bottomVertex{ XMVECTOR{ 0.0f, -radius, 0.0f, 1.0f }, XMVECTOR{ 0.0f, -1.0f, 0.0f } };
  mesh.vertices.push_back(topVertex);

  float phiStep = XM_PI / stackCount;
  float thetaStep = 2.0f*XM_PI / sliceCount;

  // Compute vertices for each stack ring (do not count the poles as rings).
  for (auto i = 1; i <= stackCount - 1; ++i) {
    float phi = i*phiStep;

    // Vertices of ring.
    for (auto j = 0; j <= sliceCount; ++j) {
      float theta = j*thetaStep;

      // spherical to cartesian
      Vertex v;
      v.position = XMVECTOR{ 
        radius*sinf(phi)*cosf(theta), 
        radius*cosf(phi), 
        radius*sinf(phi)*sinf(theta), 
        1.0f };
      v.normal = v.position;

      mesh.vertices.push_back(v);
    }
  }

  mesh.vertices.push_back(bottomVertex);

  //
  // Compute indices for top stack.  The top stack was written first to the vertex buffer
  // and connects the top pole to the first ring.
  //

  for (int i = 1; i <= sliceCount; ++i) {
    mesh.indices.push_back(0);
    mesh.indices.push_back(i + 1);
    mesh.indices.push_back(i);
  }

  //
  // Compute indices for inner stacks (not connected to poles).
  //

  // Offset the indices to the index of the first vertex in the first ring.
  // This is just skipping the top pole vertex.
  int baseIndex = 1;
  int ringVertexCount = sliceCount + 1;
  for (int i = 0; i < stackCount - 2; ++i) {
    for (int j = 0; j < sliceCount; ++j) {
      mesh.indices.push_back(baseIndex + i*ringVertexCount + j);
      mesh.indices.push_back(baseIndex + i*ringVertexCount + j + 1);
      mesh.indices.push_back(baseIndex + (i + 1)*ringVertexCount + j);
               
      mesh.indices.push_back(baseIndex + (i + 1)*ringVertexCount + j);
      mesh.indices.push_back(baseIndex + i*ringVertexCount + j + 1);
      mesh.indices.push_back(baseIndex + (i + 1)*ringVertexCount + j + 1);
    }
  }

  //
  // Compute indices for bottom stack.  The bottom stack was written last to the vertex buffer
  // and connects the bottom pole to the bottom ring.
  //

  // South pole vertex was added last.
  int southPoleIndex = static_cast<int>(mesh.vertices.size() - 1);

  // Offset the indices to the index of the first vertex in the last ring.
  baseIndex = southPoleIndex - ringVertexCount;

  for (int i = 0; i < sliceCount; ++i) {
    mesh.indices.push_back(southPoleIndex);
    mesh.indices.push_back(baseIndex + i);
    mesh.indices.push_back(baseIndex + i + 1);
  }

  return mesh;
}

// Creates an mxn grid in the xz-plane with rows rows and cols columns, centered
// at the origin with the specified width and depth.
Model::Mesh CreateGrid(float width, float depth, int rows, int cols) {
  Model::Mesh mesh;

  //
  // Calculates vertices.
  //

  const int dx = static_cast<int>(width / (cols - 1));
  const int dz = static_cast<int>(depth / (rows - 1));

  const float du = 1.0f / (cols - 1);
  const float dv = 1.0f / (rows - 1);

  float half_width = width / 2.0f;
  float half_depth = depth / 2.0f;

  // From forward to back.
  for (int i = 0; i < rows; i++) {
    const float z = half_depth - i * dz;
    // From left to right
    for (int j = 0; j < cols; j++) {
      const float x = -half_width + j * dx;
      // Create vertex.
      Vertex vertex{
        XMVECTOR{ x, 0.0, z, 1.0f },
        XMVECTOR{ 0, 1.0f, 0 },
        XMFLOAT2{ du*j, dv*i} };

      mesh.vertices.push_back(vertex);
    }
  }

  //
  // Calculates indices.
  //

  // For each rect
  // From forward to backward.
  for (int i = 0; i < rows; i++) {
    // From left to right
    for (int j = 0; j < cols; j++) {
      // Get its four index.
      const int index_lf = cols * i + j;
      const int index_rf = cols * i + j + 1;
      const int index_lb = cols * (i + 1) + j; // (i + 1) * rows + j;
      const int index_rb = cols * (i + 1) + j + 1; // (i + 1) * rows + j + 1;

      // Split it to two triangles

      // Triangle 1
      mesh.indices.push_back(index_rb);
      mesh.indices.push_back(index_lb);
      mesh.indices.push_back(index_lf);

      // Triangle 2
      mesh.indices.push_back(index_lf);
      mesh.indices.push_back(index_rf);
      mesh.indices.push_back(index_rb);
    }
  }

  return mesh;
}

}  // namespace Graphics
}  // namespace LL3D
