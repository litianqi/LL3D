#pragma once

#include "Component.h"
#include "Graphics/Model.h"
#include "Graphics/Base.h"

namespace LL3D {

class Transform;

}


namespace LL3D {
namespace Graphics {

class MeshRender : private Base {
public:
  MeshRender() = default;
  MeshRender(const Mesh& mesh, const std::vector<Material>& materials);

  void setMesh(const Mesh& value);
  void setMaterial(const Material& material);

  const Mesh& mesh() const;
  const Material& material() const;
  bool mirror() const;
  bool transparent() const;
  bool opaque() const;

  void render() const noexcept;

private:
  Mesh     mesh_;
  Material material_;
  Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer_;
  Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer_;
};


class ModelRender : public Component {

public:

  enum BuiltInModel { Cube, Sphere, Grid };

  ModelRender(const Model& model);
  ModelRender(std::experimental::filesystem::path pathname);
  ModelRender(BuiltInModel type);

  const std::string& name() const;
  const std::vector<MeshRender>& meshRenders() const;
  const DirectX::BoundingBox& localBoundingBox() const;

private:

  std::string             name_;
  std::vector<MeshRender> meshRenders_;
  DirectX::BoundingBox    localBoundingBox_;

  int __debugTag = 0;
};


DirectX::BoundingOrientedBox convertToWorldBoundingBox(
  const DirectX::BoundingBox& localBoundingBox,
  const Transform& transform);

}  // namespace Graphics
}  // namespace LL3D