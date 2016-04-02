#pragma once

#include "../Component.h"
#include "Model.h"
#include "Base.h"

namespace LL3D {
namespace Graphics {

class MeshRender : private Base {
public:
  MeshRender() = default;
  MeshRender(const Mesh& mesh, const std::vector<Material>& materials);

  void SetMesh(const Mesh& value);
  void SetMaterial(const Material& value);

  const Material& GetMaterial() const;
  bool IsMirror() const;
  bool IsTransparent() const;
  bool IsOpaque() const;

  void Render() const noexcept;

private:
  Mesh     mesh_;
  Material material_;
  Microsoft::WRL::ComPtr<ID3D11Buffer> index_buffer_;
  Microsoft::WRL::ComPtr<ID3D11Buffer> vertex_buffer_;
};

class ModelRender : public Component {
public:
  enum BuiltInModel { Cube, Sphere, Grid };

  ModelRender(const Model& model);
  ModelRender(std::experimental::filesystem::path pathname);
  ModelRender(BuiltInModel type);
  std::unique_ptr<Component> Clone() override;

  const std::string& GetName() const;
  const std::vector<MeshRender>& GetMeshRenders() const;

private:
  std::string name_;
  std::vector<MeshRender> mesh_renders_;
};

}  // namespace Graphics
}  // namespace LL3D