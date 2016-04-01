#pragma once

#include <memory>
#include <list>
#include "GameObject.h"
#include "Graphics/Base.h"

namespace LL3D {

namespace Graphics {
class MeshRender;
class LightComponent;
}
class RecursiveSceneIterator;
using RenderableMesh = std::pair<Transform, const Graphics::MeshRender*>;

class Scene : private Graphics::Base {
  friend RecursiveSceneIterator;

public: 
  void AddGameObject(const GameObject& object);
  void Update();
  void Render() noexcept;

private:
  //>
  // Get active(the first) Camera. Returns nullptr if there is no Camera.
  //
  GameObject* GetCamera() noexcept;
  std::vector<RenderableMesh> GetMirrors() noexcept;
  std::vector<RenderableMesh> GetTransparents() noexcept;
  std::vector<GameObject*> GetLights() noexcept;

  std::list<GameObject> objects_;
  bool first_update_ = true;
};

}  // namespace LL3D
