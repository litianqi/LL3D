#include "Graphics/Particle/ParticleSystem.h"
#include <d3d11.h>
#include <DDSTextureLoader.h>
#include "Core/Exceptions.h"
#include "Transform.h"
#include "GameObject.h"
#include "Graphics/Particle/ParticleEffect.h"
#include "Graphics/Particle/ParticleVertex.h"
#include "Graphics/Device.h"
#include "Graphics/TextureHelper.h"
#include "Graphics/Camera.h"
#include "Utils/Time.h"

using namespace Microsoft::WRL;

namespace LL3D {
namespace Graphics {

ParticleSystem::ParticleSystem(const Transform& transform,
                               const GameObject* camera,
                               std::tr2::sys::path effectPath,
                               std::tr2::sys::path colorTexPath)
  : ConstCameraUser(camera)
  , transform_(transform)
  , effect_(std::make_unique<ParticleEffect>(effectPath.string()))
{
  const void* shaderBytecode;
  size_t shaderBytecodeSize;
  effect_->vertexShaderBytecode(&shaderBytecode, &shaderBytecodeSize);
  throwIfFailed(s_graphicsDevice->device()->CreateInputLayout(
    ParticleVertex::kInputElements, ParticleVertex::kInputElementCount,
    shaderBytecode, shaderBytecodeSize, &inputLayout_));

  buildBuffers();

  randomTex_ = CreateRandomTexture1D(s_graphicsDevice->device());
  throwIfFailed(DirectX::CreateDDSTextureFromFile(
    s_graphicsDevice->device(), colorTexPath.wstring().c_str(), nullptr,
    &colorTex_));

  effect_->setColorTex(colorTex_.Get());
  effect_->setRandomTex(randomTex_.Get());
}

void
ParticleSystem::render()
{
  effect_->setViewProj(
    camera_->component<Graphics::Camera>()->viewProjMatrix());
  effect_->setTimeStep(static_cast<float>(Utils::Time::deltaTime().count()));
  effect_->setEmitDirWS(transform_.forwardVec());
  effect_->setEmitPosWS(transform_.position());
  effect_->setEyePosWS(camera_->transform().position());
  effect_->setRandomNumber(Math::randf());

  auto* deviceContex = s_graphicsDevice->deviceContex();

  deviceContex->IASetInputLayout(inputLayout_.Get());
  deviceContex->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

  unsigned stride = sizeof(ParticleVertex);
  auto offset = 0u;
  if (firstRun_) {
    deviceContex->IASetVertexBuffers(0, 1, initVertexBuffer_.GetAddressOf(),
                                     &stride, &offset);
  }
  else {
    deviceContex->IASetVertexBuffers(0, 1, vertexBuffer_.GetAddressOf(),
                                     &stride, &offset);
  }
  deviceContex->SOSetTargets(1, swapOutBuffer_.GetAddressOf(), &offset);

  effect_->apply(ParticleEffect::Technique::kSwapOut, deviceContex);
  if (firstRun_) {
    deviceContex->Draw(1, 0);
    firstRun_ = false;
  } else {
    deviceContex->DrawAuto();
  }

  ID3D11Buffer* emptyBuffer[1] = { nullptr };
  deviceContex->SOSetTargets(1, emptyBuffer, &offset);
  std::swap(vertexBuffer_, swapOutBuffer_);
  deviceContex->IASetVertexBuffers(0, 1, vertexBuffer_.GetAddressOf(), &stride,
                                   &offset);

  effect_->apply(ParticleEffect::Technique::kDraw, deviceContex);
  deviceContex->DrawAuto();
}

void ParticleSystem::setMaxParticles(unsigned value)
{
  maxParticles_ = value;
  buildBuffers();
}

void ParticleSystem::buildBuffers()
{
  auto desc = D3D11_BUFFER_DESC();
  desc.ByteWidth = 1 * sizeof(ParticleVertex);
  desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
  desc.Usage = D3D11_USAGE_DEFAULT;
  desc.CPUAccessFlags = 0;
  desc.MiscFlags = 0;
  desc.StructureByteStride = 0;

  auto initVertex = ParticleVertex();
  initVertex.age = 0.f;
  initVertex.flag = 0;
  auto initData = D3D11_SUBRESOURCE_DATA();
  initData.pSysMem = &initVertex;

  throwIfFailed(s_graphicsDevice->device()->CreateBuffer(&desc, &initData,
                                                         &initVertexBuffer_));

  desc.BindFlags = D3D11_BIND_VERTEX_BUFFER | D3D11_BIND_STREAM_OUTPUT;
  desc.ByteWidth = maxParticles_ * sizeof(ParticleVertex);

  throwIfFailed(
    s_graphicsDevice->device()->CreateBuffer(&desc, nullptr, &vertexBuffer_));
  throwIfFailed(
    s_graphicsDevice->device()->CreateBuffer(&desc, nullptr, &swapOutBuffer_));
}

} // namespace Graphics
} // namespace LL3D
