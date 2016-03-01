#include "Model.h"
#include <DirectXMath.h>
#include <D3D11.h>
#include <d3dx11effect.h>
#include "Core/Assert.h"
#include "Core/Exceptions.h"
#include "Color.h"
#include "Effects.h"
#include "Assets.h"

namespace LL3D {

Vertex::InputLayout::InputLayout(ID3D11Device* device, Effect* effect) {
  D3D11_INPUT_ELEMENT_DESC vertex_desc[] =
  {
    { "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 }
  };

  const void* shader_bytecode;
  size_t shader_bytecode_size;
  effect->GetVertexShaderBytecode(&shader_bytecode, &shader_bytecode_size);
  ThrowIfFailed(
    device->CreateInputLayout(vertex_desc, 3, shader_bytecode, shader_bytecode_size, &layout_)
    );
}

Vertex::InputLayout::operator ID3D11InputLayout*() {
  return layout_.Get();
}


Model::Model(ID3D11Device * device, const Mesh & mesh, DirectX::FXMMATRIX world,
  const Material & material, const std::string & texture_path) :
  mesh_(mesh), world_(world), material_(material), texture_path_(texture_path) {

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
    device->CreateBuffer(&vbd, &vinitData, &vertex_buffer_)
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
    device->CreateBuffer(&ibd, &iinitData, &index_buffer_)
    );
}

void Model::Render(ID3D11DeviceContext * device_context, BasicEffect * effect, ID3D11InputLayout * input_layout) {

  UINT stride = sizeof(Vertex);
  UINT offset = 0;
  device_context->IASetVertexBuffers(0, 1, vertex_buffer_.GetAddressOf(), &stride, &offset);
  device_context->IASetIndexBuffer(index_buffer_.Get(), DXGI_FORMAT_R32_UINT, 0);

  //for (UINT pass = 0; pass < effect->GetPassNum(); ++pass) {
  // Set per object constant buffer.
  effect->SetWorld(world_);
  effect->SetMaterial(material_);
  effect->SetTextureTransform(XMMatrixIdentity());
  effect->SetTexture(Assets::Instance()->GetTexture(texture_path_));

  // Draw object.
  effect->Apply(device_context);
  device_context->DrawIndexed(static_cast<UINT>(mesh_.indices.size()), 0, 0);
  //}
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

// Creates an mxn grid in the xz-plane with m rows and n columns, centered
// at the origin with the specified width and depth.
Model::Mesh CreateGrid(float width, float depth, int m, int n) {
  Model::Mesh mesh;

  //
  // Calculates vertices.
  //

  const int cnt_rects_x = m - 1;
  const int cnt_rects_z = n - 1;

  float x_lb = -1.0f * width / 2.0f;
  float z_lb = -1.0f * depth / 2.0f;

  // From bottom to top
  for (int i = 0; i < n; i++) {
    // Detemine z
    const float z = z_lb + i * (depth / cnt_rects_z);
    // From left to right
    for (int j = 0; j < m; j++) {
      // Determine x
      const float x = x_lb + j * (width / cnt_rects_x);
      // Create vertex.
      Vertex vertex{
        XMVECTOR{ x, 0.0, z, 1.0f },
        XMVECTOR{ 0, 1.0f, 0 } };

      mesh.vertices.push_back(vertex);
    }
  }

  //
  // Calculates indices.
  //

  // For each rect
  for (int i = 0; i < cnt_rects_z; i++) {
    for (int j = 0; j < cnt_rects_x; j++) {
      // Get its four index.
      const int index_lb = i * n + j;
      const int index_rb = i * n + j + 1;
      const int index_lt = (i + 1) * m + j;
      const int index_rt = (i + 1) * m + j + 1;

      // Split it to two triangles

      // Triangle 1
      mesh.indices.push_back(index_rb);
      mesh.indices.push_back(index_lb);
      mesh.indices.push_back(index_lt);

      // Triangle 2
      mesh.indices.push_back(index_lt);
      mesh.indices.push_back(index_rt);
      mesh.indices.push_back(index_rb);
    }
  }

  return mesh;
}

}  // namespace LL3D
