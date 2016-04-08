#include "Graphics/ModelRender.h"
#include <sstream>
#include "Core/Exceptions.h"
#include "Device.h"
#include "Effects.h"
#include "Assets.h"
#include "Core/Assert.h"

using namespace std::experimental;

namespace LL3D {
namespace Graphics {

MeshRender::MeshRender(const Mesh& mesh, const std::vector<Material>& materials) :
  material_(materials[mesh.material_index])
{
  SetMesh(mesh);
}

void MeshRender::SetMesh(const Mesh & mesh)
{
  mesh_ = mesh;

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
}

void MeshRender::SetMaterial(const Material & value)
{
  material_ = value;
}

void MeshRender::Render() const noexcept
{
  UINT stride = sizeof(Vertex);
  UINT offset = 0;

  s_graphics_device->GetDeviceContex()->IASetVertexBuffers(0, 1, 
    vertex_buffer_.GetAddressOf(), &stride, &offset);
  s_graphics_device->GetDeviceContex()->IASetIndexBuffer(index_buffer_.Get(), 
    DXGI_FORMAT_R32_UINT, 0);

  //for (UINT pass = 0; pass < effect->GetPassNum(); ++pass) {
  // Set per object constant buffer.
  s_effect->SetMaterial(MaterialFX(material_));
  //s_effect->SetTextureTransform(texture_transform_);
  if (filesystem::exists(material_.diffuse_texture))
    s_effect->SetTexture(LoadDDSFromFile(s_graphics_device->GetDevice(), material_.diffuse_texture));
  else
    s_effect->SetTexture(nullptr);

  s_effect->SetTextureTransform(DirectX::XMMatrixIdentity());

  // Apply rasterizer option, if specified.
  //s_graphics_device->GetDeviceContex()->RSSetState(rasterizer_state_.Get());

  // Draw object.
  s_effect->Apply(s_graphics_device->GetDeviceContex());
  s_graphics_device->GetDeviceContex()->DrawIndexed(
    static_cast<UINT>(mesh_.indices.size()), 0, 0);

  // Roll back rasterizer option. For it's global so will affect other models.
  //s_graphics_device->GetDeviceContex()->RSSetState(0);
}

const Material & MeshRender::GetMaterial() const
{
  return material_;
}

bool MeshRender::IsMirror() const
{
  return material_.is_mirror;
}

bool MeshRender::IsTransparent() const
{
  return !material_.is_mirror && 
    (0.f < material_.opacity && material_.opacity < 1.f);
}

bool MeshRender::IsOpaque() const
{
  return material_.opacity >= 1.f;
}

ModelRender::ModelRender(const Model & model) :
  name_(model.name)
{
  for (const auto& m : model.meshes) {
    mesh_renders_.push_back(MeshRender(m, model.materials));
  }
}

ModelRender::ModelRender(std::experimental::filesystem::path pathname)
{
  auto model = Model::LoadAssimp(pathname);
  for (const auto& m : model.meshes) {
    mesh_renders_.push_back(MeshRender(m, model.materials));
  }
  name_ = model.name;
}

ModelRender::ModelRender(BuiltInModel type)
{
  auto model = Model();
  if (type == Cube) {
    auto mesh = Mesh::CreateCube(10.f, 10.f, 10.f);
    mesh.material_index = 0;
    model.meshes.push_back(std::move(mesh));
    model.name = "Cube";
  }
  else if (type == Sphere) {
    auto mesh = Mesh::CreateSphere(5.f, 50, 50);
    mesh.material_index = 0;
    model.meshes.push_back(std::move(mesh));
    model.name = "Sphere";
  }
  else if (type == Grid) {
    auto mesh = Mesh::CreateGrid(100.f, 100.f, 2, 2);
    mesh.material_index = 0;
    model.meshes.push_back(std::move(mesh));
    model.name = "Grid";
  }
  else {
    ASSERT(false && "Wrong parameter, not in range");
  }
  
  auto mat = Material();
  mat.ambient = Math::Vector3::One;
  mat.diffuse = Math::Vector3::One;
  mat.specular = Math::Vector3::One;
  mat.emissive = Math::Vector3::One;
  mat.transparent = Math::Vector3::One;
  mat.shininess = 5.f;
  mat.opacity = 1.f;
  mat.shininess_strength = 1.f;
  model.materials.push_back(mat);
  
  for (const auto& m : model.meshes) {
    mesh_renders_.push_back(MeshRender(m, model.materials));
  }

  name_ = model.name;
}

std::unique_ptr<Component> ModelRender::Clone()
{
  return std::make_unique<ModelRender>(*this);
}

const std::string & ModelRender::GetName() const
{
  return name_;
}

const std::vector<MeshRender>& ModelRender::GetMeshRenders() const
{
  return mesh_renders_;
}

}  // namespace Graphics
}  // namespace LL3D