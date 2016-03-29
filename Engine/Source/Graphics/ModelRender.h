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
  Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizer_state_;
};

class ModelRender : public Component {
public:
  enum BuiltInType { Cube, Sphere, Grid };

  ModelRender(std::experimental::filesystem::path pathname);
  ModelRender(BuiltInType type);
  std::unique_ptr<Component> Clone() override;

  //>
  // Renders this Model.
  //
  void Update() override;

private:
  std::vector<MeshRender> mesh_renders_;
};

}  // namespace Graphics
}  // namespace LL3D