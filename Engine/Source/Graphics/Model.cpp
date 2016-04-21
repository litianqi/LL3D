#include "Graphics/Model.h"
#include <filesystem>
#include <plog/Log.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Core/Assert.h"

using namespace std;
using namespace std::experimental;
using namespace DirectX;
using namespace LL3D;
using namespace LL3D::Graphics;
using namespace LL3D::Math;

namespace {

void
BuildCylinderTopCap(float bottomRadius, float topRadius, float height,
                    UINT sliceCount, UINT stackCount, Mesh& meshData)
{
  UINT baseIndex = (UINT)meshData.vertices.size();

  float y = 0.5f * height;
  float dTheta = 2.0f * XM_PI / sliceCount;

  // Duplicate cap ring vertices because the texture coordinates and normals
  // differ.
  for (UINT i = 0; i <= sliceCount; ++i) {
    float x = topRadius * cosf(i * dTheta);
    float z = topRadius * sinf(i * dTheta);

    // Scale down by the height to try and make top cap texture coord area
    // proportional to base.
    float u = x / height + 0.5f;
    float v = z / height + 0.5f;

    meshData.vertices.push_back(Vertex{ Math::Vector3(x, y, z),
                                        Math::Vector3(0.0f, 1.0f, 0.0f),
                                        Math::Vector2(u, v) });
  }

  // Cap center vertex.
  meshData.vertices.push_back(Vertex{ Math::Vector3(0.0f, y, 0.0f),
                                      Math::Vector3(0.0f, 1.0f, 0.0f),
                                      Math::Vector2(0.5f, 0.5f) });

  // Index of center vertex.
  UINT centerIndex = (UINT)meshData.vertices.size() - 1;

  for (UINT i = 0; i < sliceCount; ++i) {
    meshData.indices.push_back(centerIndex);
    meshData.indices.push_back(baseIndex + i + 1);
    meshData.indices.push_back(baseIndex + i);
  }
}

void
BuildCylinderBottomCap(float bottomRadius, float topRadius, float height,
                       UINT sliceCount, UINT stackCount, Mesh& meshData)
{
  //
  // Build bottom cap.
  //

  UINT baseIndex = (UINT)meshData.vertices.size();
  float y = -0.5f * height;

  // vertices of ring
  float dTheta = 2.0f * XM_PI / sliceCount;
  for (UINT i = 0; i <= sliceCount; ++i) {
    float x = bottomRadius * cosf(i * dTheta);
    float z = bottomRadius * sinf(i * dTheta);

    // Scale down by the height to try and make top cap texture coord area
    // proportional to base.
    float u = x / height + 0.5f;
    float v = z / height + 0.5f;

    meshData.vertices.push_back(Vertex{ Math::Vector3(x, y, z),
                                        Math::Vector3(0.0f, -1.0f, 0.0f),
                                        Math::Vector2(u, v) });
  }

  // Cap center vertex.
  meshData.vertices.push_back(Vertex{ Math::Vector3(0.0f, y, 0.0f),
                                      Math::Vector3(0.0f, -1.0f, 0.0f),
                                      Math::Vector2(0.5f, 0.5f) });

  // Cache the index of center vertex.
  UINT centerIndex = (UINT)meshData.vertices.size() - 1;

  for (UINT i = 0; i < sliceCount; ++i) {
    meshData.indices.push_back(centerIndex);
    meshData.indices.push_back(baseIndex + i);
    meshData.indices.push_back(baseIndex + i + 1);
  }
}

} // namespace

namespace LL3D {
namespace Graphics {

Model::Model(const string& _name, const std::vector<Mesh>& _meshes,
             const std::vector<Material>& _materials)
  : name(_name)
  , meshes(_meshes)
  , materials(_materials)
  , boundingBox(calculateBoundingBox(meshes))
{
}

DirectX::BoundingBox
Model::calculateBoundingBox(const std::vector<Mesh>& meshes)
{
  auto model_box = BoundingBox();
  if (meshes.size() > 0) {
    for (const auto& mesh : meshes) {
      BoundingBox::CreateMerged(model_box, model_box, mesh.boudingBox());
    }
  }
  return model_box;
}

Model
Model::loadAssimp(filesystem::path pathname)
{
  if (!filesystem::exists(pathname))
    throw std::exception("File doesn't exists");

  Assimp::Importer importer;

  // remove unused data
  importer.SetPropertyInteger(AI_CONFIG_PP_RVC_FLAGS, aiComponent_COLORS |
                                                        aiComponent_LIGHTS |
                                                        aiComponent_CAMERAS);

  // max triangles and vertices per mesh, splits above this threshold
  importer.SetPropertyInteger(AI_CONFIG_PP_SLM_TRIANGLE_LIMIT, INT_MAX);
  importer.SetPropertyInteger(AI_CONFIG_PP_SLM_VERTEX_LIMIT,
                              0xfffe); // avoid the primitive restart index

  // remove points and lines
  importer.SetPropertyInteger(AI_CONFIG_PP_SBP_REMOVE,
                              aiPrimitiveType_POINT | aiPrimitiveType_LINE);

  const aiScene* scene = importer.ReadFile(
    pathname.string(), // todo: filename u8string?
    aiProcess_CalcTangentSpace | aiProcess_JoinIdenticalVertices |
      aiProcess_Triangulate | aiProcess_FixInfacingNormals |
      aiProcess_RemoveComponent | aiProcess_GenSmoothNormals |
      aiProcess_SplitLargeMeshes | aiProcess_ValidateDataStructure |
      // aiProcess_ImproveCacheLocality | // handled by optimizePostTransform()
      aiProcess_RemoveRedundantMaterials | aiProcess_SortByPType |
      aiProcess_FindInvalidData | aiProcess_GenUVCoords |
      aiProcess_TransformUVCoords | aiProcess_OptimizeMeshes |
      aiProcess_OptimizeGraph | aiProcess_ConvertToLeftHanded);

  if (scene == nullptr)
    throw std::exception("scene == nullptr, empty or corrupted file?");

  std::vector<Mesh> meshes;
  std::vector<Material> materials;

  if (scene->HasTextures()) {
    // embedded textures...
  }

  if (scene->HasAnimations()) {
    // todo
  }

  for (unsigned int mat_index = 0; mat_index < scene->mNumMaterials;
       mat_index++) {
    const aiMaterial* srcmat = scene->mMaterials[mat_index];
    auto dstmat = Material();

    aiColor3D diffuse(1.0f, 1.0f, 1.0f);
    aiColor3D specular(1.0f, 1.0f, 1.0f);
    aiColor3D ambient(1.0f, 1.0f, 1.0f);
    aiColor3D emissive(0.0f, 0.0f, 0.0f);
    aiColor3D transparent(1.0f, 1.0f, 1.0f);
    float opacity = 1.0f;
    float shininess = 0.0f;
    float shininess_strength = 1.0f;
    aiString texDiffusePath;
    aiString texSpecularPath;
    aiString texEmissivePath;
    aiString texNormalPath;
    aiString texLightmapPath;
    aiString texReflectionPath;
    srcmat->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse);
    srcmat->Get(AI_MATKEY_COLOR_SPECULAR, specular);
    srcmat->Get(AI_MATKEY_COLOR_AMBIENT, ambient);
    srcmat->Get(AI_MATKEY_COLOR_EMISSIVE, emissive);
    srcmat->Get(AI_MATKEY_COLOR_TRANSPARENT, transparent);
    srcmat->Get(AI_MATKEY_OPACITY, opacity);
    srcmat->Get(AI_MATKEY_SHININESS, shininess);
    srcmat->Get(AI_MATKEY_SHININESS_STRENGTH, shininess_strength);
    srcmat->Get(AI_MATKEY_TEXTURE(aiTextureType_DIFFUSE, 0), texDiffusePath);
    srcmat->Get(AI_MATKEY_TEXTURE(aiTextureType_SPECULAR, 0), texSpecularPath);
    srcmat->Get(AI_MATKEY_TEXTURE(aiTextureType_EMISSIVE, 0), texEmissivePath);
    srcmat->Get(AI_MATKEY_TEXTURE(aiTextureType_NORMALS, 0), texNormalPath);
    srcmat->Get(AI_MATKEY_TEXTURE(aiTextureType_LIGHTMAP, 0), texLightmapPath);
    srcmat->Get(AI_MATKEY_TEXTURE(aiTextureType_REFLECTION, 0),
                texReflectionPath);

    dstmat.diffuse = Vector3(diffuse.r, diffuse.g, diffuse.b);
    dstmat.specular = Vector3(specular.r, specular.g, specular.b);
    dstmat.ambient = Vector3(ambient.r, ambient.g, ambient.b);
    dstmat.emissive = Vector3(emissive.r, emissive.g, emissive.b);
    dstmat.transparent = Vector3(transparent.r, transparent.g, transparent.b);
    dstmat.opacity = opacity;
    dstmat.shininess = shininess;
    dstmat.shininess_strength = shininess_strength;

    char* pRem = nullptr;

    if (texDiffusePath.length > 0)
      dstmat.diffuseTexture =
        pathname.parent_path() / "Textures/" / texDiffusePath.C_Str();
    if (texSpecularPath.length > 0)
      dstmat.specularTexture =
        pathname.parent_path() / "Textures/" / texSpecularPath.C_Str();
    if (texEmissivePath.length > 0)
      dstmat.emissiveTexture =
        pathname.parent_path() / "Textures/" / texEmissivePath.C_Str();
    if (texNormalPath.length > 0)
      dstmat.normalTexture =
        pathname.parent_path() / "Textures/" / texNormalPath.C_Str();
    if (texLightmapPath.length > 0)
      dstmat.lightmapTexture =
        pathname.parent_path() / "Textures/" / texLightmapPath.C_Str();
    if (texReflectionPath.length > 0)
      dstmat.reflectionTexture =
        pathname.parent_path() / "Textures/" / texReflectionPath.C_Str();

    aiString matName;
    srcmat->Get(AI_MATKEY_NAME, matName);
    dstmat.name = matName.C_Str();

    materials.push_back(dstmat);
  }

  // fill in vertex and index data
  for (unsigned int mesh_index = 0; mesh_index < scene->mNumMeshes;
       mesh_index++) {
    const aiMesh* srcmesh = scene->mMeshes[mesh_index];
    auto dstmesh = Mesh();

    ASSERT(srcmesh->mPrimitiveTypes == aiPrimitiveType_TRIANGLE);

    dstmesh.materialIndex = srcmesh->mMaterialIndex;

    for (unsigned int v = 0; v < srcmesh->mNumVertices; v++) {
      Vertex vertex;

      if (srcmesh->mVertices) {
        vertex.position =
          Vector3(srcmesh->mVertices[v].x, srcmesh->mVertices[v].y,
                  srcmesh->mVertices[v].z);
      } else {
        // no position? That's kind of bad.
        ASSERT(0);
      }

      if (srcmesh->mTextureCoords[0]) {
        vertex.texcoord = Vector2(srcmesh->mTextureCoords[0][v].x,
                                  srcmesh->mTextureCoords[0][v].y);
      }

      if (srcmesh->mNormals) {
        vertex.normal = Vector3(srcmesh->mNormals[v].x, srcmesh->mNormals[v].y,
                                srcmesh->mNormals[v].z);
      } else {
        // Assimp should generate normals if they are missing, according to the
        // postprocessing flag specified on load,
        // so we should never get here.
        ASSERT(0);
      }

      if (srcmesh->mTangents) {
        vertex.tangent =
          Vector3(srcmesh->mTangents[v].x, srcmesh->mTangents[v].y,
                  srcmesh->mTangents[v].z);
      }

      if (srcmesh->mBitangents) {
        vertex.bitangent =
          Vector3(srcmesh->mBitangents[v].x, srcmesh->mBitangents[v].y,
                  srcmesh->mBitangents[v].z);
      }

      dstmesh.vertices.push_back(vertex);
    }

    for (unsigned int f = 0; f < srcmesh->mNumFaces; f++) {
      ASSERT(srcmesh->mFaces[f].mNumIndices == 3);

      dstmesh.indices.push_back(srcmesh->mFaces[f].mIndices[0]);
      dstmesh.indices.push_back(srcmesh->mFaces[f].mIndices[1]);
      dstmesh.indices.push_back(srcmesh->mFaces[f].mIndices[2]);
    }

    meshes.push_back(dstmesh);
  }

  return Model("", std::move(meshes), std::move(materials));
}

DirectX::BoundingBox
Mesh::boudingBox() const
{
  auto box = BoundingBox();
  BoundingBox::CreateFromPoints(
    box, vertices.size(), reinterpret_cast<const XMFLOAT3*>(vertices.data()),
    sizeof(Vertex));

  return box;
}

Mesh
Mesh::createCube(float width, float height, float depth)
{
  auto mesh = Mesh();

  //
  // Create the vertices.
  //

  float w2 = 0.5f * width;
  float h2 = 0.5f * height;
  float d2 = 0.5f * depth;

  // Create the back face vertex data.
  mesh.vertices.push_back(Vertex{ Vector3{ -w2, -h2, -d2 },
                                  Vector3{ 0, 0, -1.0f }, Vector2{ 1, 1 } });
  mesh.vertices.push_back(Vertex{ Vector3{ -w2, +h2, -d2 },
                                  Vector3{ 0, 0, -1.0f }, Vector2{ 0, 1 } });
  mesh.vertices.push_back(Vertex{ Vector3{ +w2, +h2, -d2 },
                                  Vector3{ 0, 0, -1.0f }, Vector2{ 0, 0 } });
  mesh.vertices.push_back(Vertex{ Vector3{ +w2, -h2, -d2 },
                                  Vector3{ 0, 0, -1.0f }, Vector2{ 1, 0 } });

  // Create the front face vertex data.
  mesh.vertices.push_back(
    Vertex{ Vector3{ -w2, -h2, +d2 }, Vector3{ 0, 0, 1.0f }, Vector2{ 0, 1 } });
  mesh.vertices.push_back(
    Vertex{ Vector3{ +w2, -h2, +d2 }, Vector3{ 0, 0, 1.0f }, Vector2{ 0, 0 } });
  mesh.vertices.push_back(
    Vertex{ Vector3{ +w2, +h2, +d2 }, Vector3{ 0, 0, 1.0f }, Vector2{ 1, 0 } });
  mesh.vertices.push_back(
    Vertex{ Vector3{ -w2, +h2, +d2 }, Vector3{ 0, 0, 1.0f }, Vector2{ 1, 1 } });

  // Create the top face vertex data.
  mesh.vertices.push_back(
    Vertex{ Vector3{ -w2, +h2, -d2 }, Vector3{ 0, 1.0f, 0 }, Vector2{ 0, 1 } });
  mesh.vertices.push_back(
    Vertex{ Vector3{ -w2, +h2, +d2 }, Vector3{ 0, 1.0f, 0 }, Vector2{ 0, 0 } });
  mesh.vertices.push_back(
    Vertex{ Vector3{ +w2, +h2, +d2 }, Vector3{ 0, 1.0f, 0 }, Vector2{ 1, 0 } });
  mesh.vertices.push_back(
    Vertex{ Vector3{ +w2, +h2, -d2 }, Vector3{ 0, 1.0f, 0 }, Vector2{ 1, 1 } });

  // Create the bottom face vertex data.
  mesh.vertices.push_back(Vertex{ Vector3{ -w2, -h2, -d2 },
                                  Vector3{ 0, -1.0f, 0 }, Vector2{ 0, 1 } });
  mesh.vertices.push_back(Vertex{ Vector3{ +w2, -h2, -d2 },
                                  Vector3{ 0, -1.0f, 0 }, Vector2{ 0, 0 } });
  mesh.vertices.push_back(Vertex{ Vector3{ +w2, -h2, +d2 },
                                  Vector3{ 0, -1.0f, 0 }, Vector2{ 1, 0 } });
  mesh.vertices.push_back(Vertex{ Vector3{ -w2, -h2, +d2 },
                                  Vector3{ 0, -1.0f, 0 }, Vector2{ 1, 1 } });

  // Create the left face vertex data.
  mesh.vertices.push_back(Vertex{ Vector3{ -w2, -h2, +d2 },
                                  Vector3{ -1.0f, 0, 0 }, Vector2{ 0, 1 } });
  mesh.vertices.push_back(Vertex{ Vector3{ -w2, +h2, +d2 },
                                  Vector3{ -1.0f, 0, 0 }, Vector2{ 0, 0 } });
  mesh.vertices.push_back(Vertex{ Vector3{ -w2, +h2, -d2 },
                                  Vector3{ -1.0f, 0, 0 }, Vector2{ 1, 0 } });
  mesh.vertices.push_back(Vertex{ Vector3{ -w2, -h2, -d2 },
                                  Vector3{ -1.0f, 0, 0 }, Vector2{ 1, 1 } });

  // Create the right face vertex data.
  mesh.vertices.push_back(
    Vertex{ Vector3{ +w2, -h2, -d2 }, Vector3{ 1.0f, 0, 0 }, Vector2{ 0, 1 } });
  mesh.vertices.push_back(
    Vertex{ Vector3{ +w2, +h2, -d2 }, Vector3{ 1.0f, 0, 0 }, Vector2{ 0, 0 } });
  mesh.vertices.push_back(
    Vertex{ Vector3{ +w2, +h2, +d2 }, Vector3{ 1.0f, 0, 0 }, Vector2{ 1, 0 } });
  mesh.vertices.push_back(
    Vertex{ Vector3{ +w2, -h2, +d2 }, Vector3{ 1.0f, 0, 0 }, Vector2{ 1, 1 } });

  //
  // Create the indices.
  //

  mesh.indices.resize(36);

  // Fill in the front face index data
  mesh.indices[0] = 0;
  mesh.indices[1] = 1;
  mesh.indices[2] = 2;
  mesh.indices[3] = 0;
  mesh.indices[4] = 2;
  mesh.indices[5] = 3;

  // Fill in the back face index data
  mesh.indices[6] = 4;
  mesh.indices[7] = 5;
  mesh.indices[8] = 6;
  mesh.indices[9] = 4;
  mesh.indices[10] = 6;
  mesh.indices[11] = 7;

  // Fill in the top face index data
  mesh.indices[12] = 8;
  mesh.indices[13] = 9;
  mesh.indices[14] = 10;
  mesh.indices[15] = 8;
  mesh.indices[16] = 10;
  mesh.indices[17] = 11;

  // Fill in the bottom face index data
  mesh.indices[18] = 12;
  mesh.indices[19] = 13;
  mesh.indices[20] = 14;
  mesh.indices[21] = 12;
  mesh.indices[22] = 14;
  mesh.indices[23] = 15;

  // Fill in the left face index data
  mesh.indices[24] = 16;
  mesh.indices[25] = 17;
  mesh.indices[26] = 18;
  mesh.indices[27] = 16;
  mesh.indices[28] = 18;
  mesh.indices[29] = 19;

  // Fill in the right face index data
  mesh.indices[30] = 20;
  mesh.indices[31] = 21;
  mesh.indices[32] = 22;
  mesh.indices[33] = 20;
  mesh.indices[34] = 22;
  mesh.indices[35] = 23;

  return mesh;
}

Mesh
Mesh::createSphere(float radius, int sliceCount, int stackCount)
{
  auto mesh = Mesh();

  //
  // Compute the vertices stating at the top pole and moving down the stacks.
  //

  // Poles: note that there will be texture coordinate distortion as there is
  // not a unique point on the texture map to assign to the pole when mapping
  // a rectangular texture onto a sphere.
  Vertex topVertex{ Vector3{ 0.0f, +radius, 0.0f },
                    Vector3{ 0.0f, 1.0f, 0.0f } };
  Vertex bottomVertex{ Vector3{ 0.0f, -radius, 0.0f },
                       Vector3{ 0.0f, -1.0f, 0.0f } };
  mesh.vertices.push_back(topVertex);

  float phiStep = DirectX::XM_PI / stackCount;
  float thetaStep = 2.0f * DirectX::XM_PI / sliceCount;

  // Compute vertices for each stack ring (do not count the poles as rings).
  for (auto i = 1; i <= stackCount - 1; ++i) {
    float phi = i * phiStep;

    // vertices of ring.
    for (auto j = 0; j <= sliceCount; ++j) {
      float theta = j * thetaStep;

      // spherical to cartesian
      Vertex v;
      v.position = Vector3(radius * sinf(phi) * cosf(theta), radius * cosf(phi),
                           radius * sinf(phi) * sinf(theta));
      v.normal = v.position;

      mesh.vertices.push_back(v);
    }
  }

  mesh.vertices.push_back(bottomVertex);

  //
  // Compute indices for top stack.  The top stack was written first to the
  // vertex buffer
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
      mesh.indices.push_back(baseIndex + i * ringVertexCount + j);
      mesh.indices.push_back(baseIndex + i * ringVertexCount + j + 1);
      mesh.indices.push_back(baseIndex + (i + 1) * ringVertexCount + j);

      mesh.indices.push_back(baseIndex + (i + 1) * ringVertexCount + j);
      mesh.indices.push_back(baseIndex + i * ringVertexCount + j + 1);
      mesh.indices.push_back(baseIndex + (i + 1) * ringVertexCount + j + 1);
    }
  }

  //
  // Compute indices for bottom stack.  The bottom stack was written last to the
  // vertex buffer
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

Mesh
Mesh::createCylinder(float bottomRadius, float topRadius, float height,
                     UINT sliceCount, UINT stackCount)
{
  auto meshData = Mesh();

  //
  // Build Stacks.
  //

  float stackHeight = height / stackCount;

  // Amount to increment radius as we move up each stack level from bottom to
  // top.
  float radiusStep = (topRadius - bottomRadius) / stackCount;

  UINT ringCount = stackCount + 1;

  // Compute vertices for each stack ring starting at the bottom and moving up.
  for (UINT i = 0; i < ringCount; ++i) {
    float y = -0.5f * height + i * stackHeight;
    float r = bottomRadius + i * radiusStep;

    // vertices of ring
    float dTheta = 2.0f * XM_PI / sliceCount;
    for (UINT j = 0; j <= sliceCount; ++j) {
      Vertex vertex;

      float c = cosf(j * dTheta);
      float s = sinf(j * dTheta);

      vertex.position = XMFLOAT3(r * c, y, r * s);

      vertex.texcoord.x = (float)j / sliceCount;
      vertex.texcoord.y = 1.0f - (float)i / stackCount;

      // Cylinder can be parameterized as follows, where we introduce v
      // parameter that goes in the same direction as the v tex-coord
      // so that the bitangent goes in the same direction as the v tex-coord.
      //   Let r0 be the bottom radius and let r1 be the top radius.
      //   y(v) = h - hv for v in [0,1].
      //   r(v) = r1 + (r0-r1)v
      //
      //   x(t, v) = r(v)*cos(t)
      //   y(t, v) = h - hv
      //   z(t, v) = r(v)*sin(t)
      //
      //  dx/dt = -r(v)*sin(t)
      //  dy/dt = 0
      //  dz/dt = +r(v)*cos(t)
      //
      //  dx/dv = (r0-r1)*cos(t)
      //  dy/dv = -h
      //  dz/dv = (r0-r1)*sin(t)

      // This is unit length.
      vertex.tangent = XMFLOAT3(-s, 0.0f, c);

      float dr = bottomRadius - topRadius;
      XMFLOAT3 bitangent(dr * c, -height, dr * s);

      XMVECTOR T = XMLoadFloat3(&vertex.tangent);
      XMVECTOR B = XMLoadFloat3(&bitangent);
      XMVECTOR N = XMVector3Normalize(XMVector3Cross(T, B));
      XMStoreFloat3(&vertex.normal, N);

      meshData.vertices.push_back(vertex);
    }
  }

  // Add one because we duplicate the first and last vertex per ring
  // since the texture coordinates are different.
  UINT ringVertexCount = sliceCount + 1;

  // Compute indices for each stack.
  for (UINT i = 0; i < stackCount; ++i) {
    for (UINT j = 0; j < sliceCount; ++j) {
      meshData.indices.push_back(i * ringVertexCount + j);
      meshData.indices.push_back((i + 1) * ringVertexCount + j);
      meshData.indices.push_back((i + 1) * ringVertexCount + j + 1);

      meshData.indices.push_back(i * ringVertexCount + j);
      meshData.indices.push_back((i + 1) * ringVertexCount + j + 1);
      meshData.indices.push_back(i * ringVertexCount + j + 1);
    }
  }

  BuildCylinderTopCap(bottomRadius, topRadius, height, sliceCount, stackCount,
                      meshData);
  BuildCylinderBottomCap(bottomRadius, topRadius, height, sliceCount,
                         stackCount, meshData);

  return meshData;
}

Mesh
Mesh::createGrid(float width, float depth, int cols, int rows)
{
  auto mesh = Mesh();

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
      Vertex vertex{ Vector3{ x, 0.0, z }, Vector3{ 0, 1.0f, 0 },
                     Vector2{ du * j, dv * i } };

      mesh.vertices.push_back(vertex);
    }
  }

  //
  // Calculates indices.
  //

  // For each rect
  // From forward to backward.
  for (int i = 0; i < rows - 1; i++) {
    // From left to right
    for (int j = 0; j < cols - 1; j++) {
      // Get its four index.
      const int index_lf = cols * i + j;
      const int index_rf = cols * i + j + 1;
      const int index_lb = cols * (i + 1) + j;     // (i + 1) * rows + j;
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

} // namespace Graphics
} // namespace LL3D
