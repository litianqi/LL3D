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

  void setMaterial(const Material& material);
  void setCastShadow(bool value);
  Material& material();

  const Mesh& mesh() const;
  const Material& material() const;
  bool mirror() const;
  bool transparent() const;
  bool opaque() const;
  bool castShadow() const;

  void render() const;

  int __debugTag = 0;

private:
  void setMesh(const Mesh& value);

  Mesh mesh_;
  Material material_;
  bool castShadow_ = true;
  Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer_;
  Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer_;
};


class ModelRender : public Component {
public:
  enum BuiltInModel { kCube, kSphere, kCylinder, kGrid };
  using Iterator = std::vector<MeshRender>::iterator;
  using ConstIterator = std::vector<MeshRender>::const_iterator;

  ModelRender(const Model& model);
  ModelRender(std::experimental::filesystem::path pathname);
  ModelRender(BuiltInModel type);

  const std::string& name() const;
  Iterator begin();
  Iterator end();
  ConstIterator begin() const;
  ConstIterator end() const;
  const DirectX::BoundingBox& localBoundingBox() const;

private:
  std::string name_;
  std::vector<MeshRender> meshRenders_;
  DirectX::BoundingBox localBoundingBox_;
};


DirectX::BoundingOrientedBox convertToWorldBoundingBox(
  const DirectX::BoundingBox& localBoundingBox,
  const Transform& transform);

}  // namespace Graphics
}  // namespace LL3D