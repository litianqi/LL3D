#include "Graphics/ModelRender.h"
#include <sstream>
#include "Core/Exceptions.h"
#include "Device.h"
#include "Effects.h"
#include "Assets.h"
#include "Core/Assert.h"
#include "Transform.h"

using namespace std::experimental;

namespace LL3D {
namespace Graphics {

MeshRender::MeshRender(const Mesh& mesh, const std::vector<Material>& materials) :
  material_(materials[mesh.materialIndex])
{
  setMesh(mesh);
}

void MeshRender::setMesh(const Mesh & mesh)
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

  throwIfFailed(
    s_graphicsDevice->device()->CreateBuffer(&vbd, &vinitData, &vertexBuffer_)
    );

  D3D11_BUFFER_DESC ibd;
  ibd.Usage = D3D11_USAGE_IMMUTABLE;
  ibd.ByteWidth = static_cast<UINT>(sizeof(unsigned int) * mesh.indices.size());
  ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
  ibd.CPUAccessFlags = 0;
  ibd.MiscFlags = 0;
  D3D11_SUBRESOURCE_DATA iinitData;
  iinitData.pSysMem = mesh.indices.data();

  throwIfFailed(
    s_graphicsDevice->device()->CreateBuffer(&ibd, &iinitData, &indexBuffer_)
    );
}

void MeshRender::setMaterial(const Material & material)
{
  material_ = material;
}

void MeshRender::render() const noexcept
{
  UINT stride = sizeof(Vertex);
  UINT offset = 0;

  s_graphicsDevice->deviceContex()->IASetVertexBuffers(0, 1, 
    vertexBuffer_.GetAddressOf(), &stride, &offset);
  s_graphicsDevice->deviceContex()->IASetIndexBuffer(indexBuffer_.Get(), 
    DXGI_FORMAT_R32_UINT, 0);

  //for (UINT pass = 0; pass < effect->GetPassNum(); ++pass) {
  // Set per object constant buffer.
  s_effect->setMaterial(MaterialFX(material_));
  //s_effect->SetTextureTransform(texture_transform_);
  if (filesystem::exists(material_.diffuseTexture))
    s_effect->setTexture(loadDDSFromFile(s_graphicsDevice->device(), material_.diffuseTexture));
  else
    s_effect->setTexture(nullptr);

  s_effect->setTextureTransform(DirectX::XMMatrixIdentity());

  // Apply rasterizer option, if specified.
  //s_graphics_device->GetDeviceContex()->RSSetState(rasterizer_state_.Get());

  // Draw object.
  s_effect->apply(s_graphicsDevice->deviceContex());
  s_graphicsDevice->deviceContex()->DrawIndexed(
    static_cast<UINT>(mesh_.indices.size()), 0, 0);

  // Roll back rasterizer option. For it's global so will affect other models.
  //s_graphics_device->GetDeviceContex()->RSSetState(0);
}

const Material & MeshRender::material() const
{
  return material_;
}

bool MeshRender::mirror() const
{
  return material_.mirror;
}

bool MeshRender::transparent() const
{
  return !material_.mirror && 
    (0.f < material_.opacity && material_.opacity < 1.f);
}

bool MeshRender::opaque() const
{
  return material_.opacity >= 1.f;
}

ModelRender::ModelRender(const Model & model) :
  name_(model.name)
{
  for (const auto& m : model.meshes) {
    meshRenders_.push_back(MeshRender(m, model.materials));
  }
}

ModelRender::ModelRender(std::experimental::filesystem::path pathname)
{
  auto model = Model::loadAssimp(pathname);
  for (const auto& m : model.meshes) {
    meshRenders_.push_back(MeshRender(m, model.materials));
  }
  name_ = model.name;
}

ModelRender::ModelRender(BuiltInModel type)
{
  auto model = Model();
  if (type == Cube) {
    auto mesh = Mesh::createCube(10.f, 10.f, 10.f);
    mesh.materialIndex = 0;
    model.meshes.push_back(std::move(mesh));
    model.name = "Cube";
  }
  else if (type == Sphere) {
    auto mesh = Mesh::createSphere(5.f, 50, 50);
    mesh.materialIndex = 0;
    model.meshes.push_back(std::move(mesh));
    model.name = "Sphere";
  }
  else if (type == Grid) {
    auto mesh = Mesh::createGrid(100.f, 100.f, 2, 2);
    mesh.materialIndex = 0;
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
    meshRenders_.push_back(MeshRender(m, model.materials));
  }

  name_ = model.name;
}

const std::string & ModelRender::name() const
{
  return name_;
}

const std::vector<MeshRender>& ModelRender::meshRenders() const
{
  return meshRenders_;
}

const DirectX::BoundingBox & ModelRender::localBoundingBox() const
{
  return localBoundingBox_;
}

DirectX::BoundingOrientedBox convertToWorldBoundingBox(
  const DirectX::BoundingBox & localBoundingBox, 
  const Transform & transform)
{
  const auto center = localBoundingBox.Center + transform.position();
  const auto extents = localBoundingBox.Extents * transform.scale();
  const auto rotation = transform.rotationQuaternion();
  
  return DirectX::BoundingOrientedBox(
      center, extents, rotation
    );
}

}  // namespace Graphics
}  // namespace LL3D