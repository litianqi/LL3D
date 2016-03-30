#pragma once

#include "../Component.h"
#include "Model.h"
#include "Base.h"

namespace LL3D {
namespace Graphics {

class MeshRender : private Base {
public:
  MeshRender(const Mesh& mesh, const std::vector<Material>& materials);
  void Render() noexcept;

private:
  const Mesh     mesh_;
  const Material material_;
  Microsoft::WRL::ComPtr<ID3D11Buffer> index_buffer_;
  Microsoft::WRL::ComPtr<ID3D11Buffer> vertex_buffer_;
};

class ModelRender : public Component {
public:
  enum BuiltInType { Cube, Sphere, Grid };

  ModelRender(const Model& model);
  ModelRender(std::experimental::filesystem::path pathname);
  ModelRender(BuiltInType type);
  std::unique_ptr<Component> Clone() override;

  bool IsTransparent() const;
  const Model& GetModel() const noexcept;
  //>
  // Renders this Model.
  //
  void Update() override;

private:
  std::string name_;
  Model model_;
  std::vector<MeshRender> mesh_renders_;
};

}  // namespace Graphics
}  // namespace LL3D