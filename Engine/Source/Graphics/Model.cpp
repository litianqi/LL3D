#include "Model.h"
#include <filesystem>
#include <plog/Log.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "../Core/Assert.h"

using namespace std;
using namespace std::experimental;
using namespace LL3D::Math;

namespace LL3D {
namespace Graphics {

Model Model::LoadAssimp(filesystem::path filename)
{
  if (!filesystem::exists(filename))
    throw std::exception("File doesn't exists");

  Assimp::Importer importer;

  // remove unused data
  importer.SetPropertyInteger(AI_CONFIG_PP_RVC_FLAGS,
    aiComponent_COLORS | aiComponent_LIGHTS | aiComponent_CAMERAS);

  // max triangles and vertices per mesh, splits above this threshold
  importer.SetPropertyInteger(AI_CONFIG_PP_SLM_TRIANGLE_LIMIT, INT_MAX);
  importer.SetPropertyInteger(AI_CONFIG_PP_SLM_VERTEX_LIMIT, 0xfffe); // avoid the primitive restart index

                                                                      // remove points and lines
  importer.SetPropertyInteger(AI_CONFIG_PP_SBP_REMOVE, aiPrimitiveType_POINT | aiPrimitiveType_LINE);

  const aiScene *scene = importer.ReadFile(filename.string(),  // todo: filename u8string?
  /*  aiProcess_CalcTangentSpace |
    aiProcess_JoinIdenticalVertices |*/
    aiProcess_Triangulate
    /*aiProcess_RemoveComponent |
    aiProcess_GenSmoothNormals |
    aiProcess_SplitLargeMeshes |
    aiProcess_ValidateDataStructure |*/
    //aiProcess_ImproveCacheLocality | // handled by optimizePostTransform()
    /*aiProcess_RemoveRedundantMaterials |
    aiProcess_SortByPType |
    aiProcess_FindInvalidData |*/
    /*aiProcess_GenUVCoords |
    aiProcess_TransformUVCoords |*/
    /*aiProcess_OptimizeMeshes |
    aiProcess_OptimizeGraph |*/
    /*aiProcess_ConvertToLeftHanded*/);

  if (scene == nullptr)
    throw std::exception("scene == nullptr, empty or corrupted file?");

  auto model = Model();

  if (scene->HasTextures())
  {
    // embedded textures...
  }

  if (scene->HasAnimations())
  {
    // todo
  }

  for (unsigned int materialIndex = 0; materialIndex < scene->mNumMaterials; materialIndex++)
  {
    const aiMaterial *srcMat = scene->mMaterials[materialIndex];
    auto dstMat = Material();

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
    srcMat->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse);
    srcMat->Get(AI_MATKEY_COLOR_SPECULAR, specular);
    srcMat->Get(AI_MATKEY_COLOR_AMBIENT, ambient);
    srcMat->Get(AI_MATKEY_COLOR_EMISSIVE, emissive);
    srcMat->Get(AI_MATKEY_COLOR_TRANSPARENT, transparent);
    srcMat->Get(AI_MATKEY_OPACITY, opacity);
    srcMat->Get(AI_MATKEY_SHININESS, shininess);
    srcMat->Get(AI_MATKEY_SHININESS_STRENGTH, shininess_strength);
    srcMat->Get(AI_MATKEY_TEXTURE(aiTextureType_DIFFUSE, 0), texDiffusePath);
    srcMat->Get(AI_MATKEY_TEXTURE(aiTextureType_SPECULAR, 0), texSpecularPath);
    srcMat->Get(AI_MATKEY_TEXTURE(aiTextureType_EMISSIVE, 0), texEmissivePath);
    srcMat->Get(AI_MATKEY_TEXTURE(aiTextureType_NORMALS, 0), texNormalPath);
    srcMat->Get(AI_MATKEY_TEXTURE(aiTextureType_LIGHTMAP, 0), texLightmapPath);
    srcMat->Get(AI_MATKEY_TEXTURE(aiTextureType_REFLECTION, 0), texReflectionPath);

    dstMat.diffuse = Vector3(diffuse.r, diffuse.g, diffuse.b);
    dstMat.specular = Vector3(specular.r, specular.g, specular.b);
    dstMat.ambient = Vector3(ambient.r, ambient.g, ambient.b);
    dstMat.emissive = Vector3(emissive.r, emissive.g, emissive.b);
    dstMat.transparent = Vector3(transparent.r, transparent.g, transparent.b);
    dstMat.opacity = opacity;
    dstMat.shininess = shininess;
    dstMat.shininess_strength = shininess_strength;

    char *pRem = nullptr;

    dstMat.diffuse_texture = filename.parent_path() / "Texture/" / texDiffusePath.C_Str();
    dstMat.specular_texture = filename.parent_path() / "Texture/" / texSpecularPath.C_Str();
    dstMat.emissive_texture = filename.parent_path() / "Texture/" / texEmissivePath.C_Str();
    dstMat.normal_texture = filename.parent_path() / "Texture/" / texNormalPath.C_Str();
    dstMat.lightmap_texture = filename.parent_path() / "Texture/" / texLightmapPath.C_Str();
    dstMat.reflection_texture = filename.parent_path() / "Texture/" / texReflectionPath.C_Str();

    aiString matName;
    srcMat->Get(AI_MATKEY_NAME, matName);
    dstMat.name = matName.C_Str();

    model.materials.push_back(dstMat);
  }

  // fill in vertex and index data
  for (unsigned int meshIndex = 0; meshIndex < scene->mNumMeshes; meshIndex++)
  {
    const aiMesh *srcMesh = scene->mMeshes[meshIndex];
    auto dstMesh = Mesh();

    ASSERT(srcMesh->mPrimitiveTypes == aiPrimitiveType_TRIANGLE);

    dstMesh.material_index = srcMesh->mMaterialIndex;

    for (unsigned int v = 0; v < srcMesh->mNumVertices; v++)
    {
      Vertex vertex;

      if (srcMesh->mVertices)
      {
        vertex.position = Vector3(srcMesh->mVertices[v].x, 
          srcMesh->mVertices[v].y, srcMesh->mVertices[v].z);
      } else
      {
        // no position? That's kind of bad.
        ASSERT(0);
      }

      if (srcMesh->mTextureCoords[0])
      {
        vertex.texcoord = Vector2(srcMesh->mTextureCoords[0][v].x,
          srcMesh->mTextureCoords[0][v].y);
      }

      if (srcMesh->mNormals)
      {
        vertex.normal = Vector3(srcMesh->mNormals[v].x, srcMesh->mNormals[v].y,
          srcMesh->mNormals[v].z);
      } else
      {
        // Assimp should generate normals if they are missing, according to the postprocessing flag specified on load,
        // so we should never get here.
        ASSERT(0);
      }

      if (srcMesh->mTangents)
      {
        vertex.tangent = Vector3(srcMesh->mTangents[v].x, srcMesh->mTangents[v].y,
          srcMesh->mTangents[v].z);
      }

      if (srcMesh->mBitangents)
      {
        vertex.bitangent = Vector3(srcMesh->mBitangents[v].x, srcMesh->mBitangents[v].y,
          srcMesh->mBitangents[v].z);
      }

      dstMesh.vertices.push_back(vertex);
    }

    for (unsigned int f = 0; f < srcMesh->mNumFaces; f++)
    {
      ASSERT(srcMesh->mFaces[f].mNumIndices == 3);

      dstMesh.indices.push_back(srcMesh->mFaces[f].mIndices[0]);
      dstMesh.indices.push_back(srcMesh->mFaces[f].mIndices[1]);
      dstMesh.indices.push_back(srcMesh->mFaces[f].mIndices[2]);
    }

    model.meshes.push_back(dstMesh);
  }

  return model;
}

Mesh Mesh::CreateBox(float width, float height, float depth)
{
  auto mesh = Mesh();

  //
  // Create the vertices.
  //

  float w2 = 0.5f*width;
  float h2 = 0.5f*height;
  float d2 = 0.5f*depth;

  // Create the back face vertex data.
  mesh.vertices.push_back(Vertex{ Vector3{ -w2, -h2, -d2 }, Vector3{ 0, 0, -1.0f }, Vector2{ 1, 1 } });
  mesh.vertices.push_back(Vertex{ Vector3{ -w2, +h2, -d2 }, Vector3{ 0, 0, -1.0f }, Vector2{ 0, 1 } });
  mesh.vertices.push_back(Vertex{ Vector3{ +w2, +h2, -d2 }, Vector3{ 0, 0, -1.0f }, Vector2{ 0, 0 } });
  mesh.vertices.push_back(Vertex{ Vector3{ +w2, -h2, -d2 }, Vector3{ 0, 0, -1.0f }, Vector2{ 1, 0 } });

  // Create the front face vertex data.
  mesh.vertices.push_back(Vertex{ Vector3{ -w2, -h2, +d2 }, Vector3{ 0, 0, 1.0f }, Vector2{ 0, 1 } });
  mesh.vertices.push_back(Vertex{ Vector3{ +w2, -h2, +d2 }, Vector3{ 0, 0, 1.0f }, Vector2{ 0, 0 } });
  mesh.vertices.push_back(Vertex{ Vector3{ +w2, +h2, +d2 }, Vector3{ 0, 0, 1.0f }, Vector2{ 1, 0 } });
  mesh.vertices.push_back(Vertex{ Vector3{ -w2, +h2, +d2 }, Vector3{ 0, 0, 1.0f }, Vector2{ 1, 1 } });

  // Create the top face vertex data.
  mesh.vertices.push_back(Vertex{ Vector3{ -w2, +h2, -d2 }, Vector3{ 0, 1.0f, 0 }, Vector2{ 0, 1 } });
  mesh.vertices.push_back(Vertex{ Vector3{ -w2, +h2, +d2 }, Vector3{ 0, 1.0f, 0 }, Vector2{ 0, 0 } });
  mesh.vertices.push_back(Vertex{ Vector3{ +w2, +h2, +d2 }, Vector3{ 0, 1.0f, 0 }, Vector2{ 1, 0 } });
  mesh.vertices.push_back(Vertex{ Vector3{ +w2, +h2, -d2 }, Vector3{ 0, 1.0f, 0 }, Vector2{ 1, 1 } });

  // Create the bottom face vertex data.                               
  mesh.vertices.push_back(Vertex{ Vector3{ -w2, -h2, -d2 }, Vector3{ 0, -1.0f, 0 }, Vector2{ 0, 1 } });
  mesh.vertices.push_back(Vertex{ Vector3{ +w2, -h2, -d2 }, Vector3{ 0, -1.0f, 0 }, Vector2{ 0, 0 } });
  mesh.vertices.push_back(Vertex{ Vector3{ +w2, -h2, +d2 }, Vector3{ 0, -1.0f, 0 }, Vector2{ 1, 0 } });
  mesh.vertices.push_back(Vertex{ Vector3{ -w2, -h2, +d2 }, Vector3{ 0, -1.0f, 0 }, Vector2{ 1, 1 } });

  // Create the left face vertex data.                           
  mesh.vertices.push_back(Vertex{ Vector3{ -w2, -h2, +d2 }, Vector3{ -1.0f, 0, 0 }, Vector2{ 0, 1 } });
  mesh.vertices.push_back(Vertex{ Vector3{ -w2, +h2, +d2 }, Vector3{ -1.0f, 0, 0 }, Vector2{ 0, 0 } });
  mesh.vertices.push_back(Vertex{ Vector3{ -w2, +h2, -d2 }, Vector3{ -1.0f, 0, 0 }, Vector2{ 1, 0 } });
  mesh.vertices.push_back(Vertex{ Vector3{ -w2, -h2, -d2 }, Vector3{ -1.0f, 0, 0 }, Vector2{ 1, 1 } });

  // Create the right face vertex data.                          
  mesh.vertices.push_back(Vertex{ Vector3{ +w2, -h2, -d2 }, Vector3{ 1.0f, 0, 0 }, Vector2{ 0, 1 } });
  mesh.vertices.push_back(Vertex{ Vector3{ +w2, +h2, -d2 }, Vector3{ 1.0f, 0, 0 }, Vector2{ 0, 0 } });
  mesh.vertices.push_back(Vertex{ Vector3{ +w2, +h2, +d2 }, Vector3{ 1.0f, 0, 0 }, Vector2{ 1, 0 } });
  mesh.vertices.push_back(Vertex{ Vector3{ +w2, -h2, +d2 }, Vector3{ 1.0f, 0, 0 }, Vector2{ 1, 1 } });


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

Mesh Mesh::CreateSphere(float radius, int sliceCount, int stackCount)
{
  auto mesh = Mesh();

  //
  // Compute the vertices stating at the top pole and moving down the stacks.
  //

  // Poles: note that there will be texture coordinate distortion as there is
  // not a unique point on the texture map to assign to the pole when mapping
  // a rectangular texture onto a sphere.
  Vertex topVertex{ Vector3{ 0.0f, +radius, 0.0f }, Vector3{ 0.0f, 1.0f, 0.0f } };
  Vertex bottomVertex{ Vector3{ 0.0f, -radius, 0.0f}, Vector3{ 0.0f, -1.0f, 0.0f } };
  mesh.vertices.push_back(topVertex);

  float phiStep = DirectX::XM_PI / stackCount;
  float thetaStep = 2.0f*DirectX::XM_PI / sliceCount;

  // Compute vertices for each stack ring (do not count the poles as rings).
  for (auto i = 1; i <= stackCount - 1; ++i) {
    float phi = i*phiStep;

    // Vertices of ring.
    for (auto j = 0; j <= sliceCount; ++j) {
      float theta = j*thetaStep;

      // spherical to cartesian
      Vertex v;
      v.position = Vector3(
        radius*sinf(phi)*cosf(theta),
        radius*cosf(phi),
        radius*sinf(phi)*sinf(theta));
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

Mesh Mesh::CreateGrid(float width, float depth, int cols, int rows)
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
      Vertex vertex{
        Vector3{ x, 0.0, z },
        Vector3{ 0, 1.0f, 0 },
        Vector2{ du*j, dv*i } };

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

