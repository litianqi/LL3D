#pragma once

#include <chrono>
#include <filesystem>
#include <wrl.h>
#include "Component.h"
#include "Graphics/CameraUser.h"
#include "Graphics/Base.h"

namespace LL3D {
namespace Graphics {
class Camera;
class ParticleEffect;
}
class Transform;
}
struct ID3D11InputLayout;
struct ID3D11Buffer;
struct ID3D11ShaderResourceView;

namespace LL3D {
namespace Graphics {

class ParticleSystem : public Component, public ConstCameraUser, private Base
{
public:
  ParticleSystem(const Transform& transform, const GameObject* camera,
                 std::tr2::sys::path effectPath,
                 std::tr2::sys::path colorTexPath);

  void render() override;

  /**
   * Change max particles num, defualt is 5000.
   */
  void setMaxParticles(unsigned value);

private:
  /**
   * Build or rebuild (if already built) buffers.
   */
  void buildBuffers();

  const Transform& transform_;
  std::unique_ptr<ParticleEffect> effect_;

  Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout_;
  Microsoft::WRL::ComPtr<ID3D11Buffer> initVertexBuffer_;
  Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer_;
  Microsoft::WRL::ComPtr<ID3D11Buffer> swapOutBuffer_;
  Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> colorTex_;
  Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> randomTex_;

  bool firstRun_ = true;
  unsigned maxParticles_ = 5000;
  std::chrono::seconds timeStep_;
};

} // namespace Graphics
} // namespace LL3D