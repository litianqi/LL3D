#include "ModelRender.h"
#include <sstream>
#include "..\Core\Exceptions.h"
#include "Device.h"
#include "Effects.h"
#include "..\Assets.h"

using namespace std::experimental;

namespace LL3D {
namespace Graphics {

MeshRender::MeshRender(const Mesh& mesh, const std::vector<Material>& materials) :
  mesh_(mesh),
  material_(materials[mesh.material_index])
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

  // todo: remove or add parameter to control.
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

void MeshRender::Render() noexcept
{
  UINT stride = sizeof(Vertex);
  UINT offset = 0;

  s_graphics_device->GetDeviceContex()->IASetVertexBuffers(0, 1, 
    vertex_buffer_.GetAddressOf(), &stride, &offset);
  s_graphics_device->GetDeviceContex()->IASetIndexBuffer(index_buffer_.Get(), 
    DXGI_FORMAT_R32_UINT, 0);

  //for (UINT pass = 0; pass < effect->GetPassNum(); ++pass) {
  // Set per object constant buffer.
  s_effect->SetMaterial(static_cast<Material::EffectBuffer>(material_));
  //s_effect->SetTextureTransform(texture_transform_);
  if (filesystem::exists(material_.diffuse_texture))
    s_effect->SetTexture(CreateTexture(s_graphics_device->GetDevice(), material_.diffuse_texture));

  s_effect->SetTextureTransform(DirectX::XMMatrixIdentity());

  // Apply rasterizer option, if specified.
  s_graphics_device->GetDeviceContex()->RSSetState(rasterizer_state_.Get());

  // Draw object.
  s_effect->Apply(s_graphics_device->GetDeviceContex());
  s_graphics_device->GetDeviceContex()->DrawIndexed(
    static_cast<UINT>(mesh_.indices.size()), 0, 0);

  // Roll back rasterizer option. For it's global so will affect other models.
  s_graphics_device->GetDeviceContex()->RSSetState(0);
}

ModelRender::ModelRender(std::experimental::filesystem::path pathname)
{
  auto model = Model::LoadAssimp(pathname);
  for (const auto& m : model.meshes) {
    mesh_renders_.push_back(MeshRender(m, model.materials));
  }
}

std::unique_ptr<Component> ModelRender::Clone()
{
  return std::make_unique<ModelRender>(*this);
}

void ModelRender::Update()
{
  for (auto& m : mesh_renders_) {
    m.Render();
  }
}

}  // namespace Graphics
}  // namespace LL3D