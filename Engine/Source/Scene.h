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
}

namespace LL3D {

class Scene : private Graphics::Base {

  friend RecursiveSceneIterator;

public: 

  void Add(std::unique_ptr<GameObject> object);
  void Remove(RecursiveSceneIterator iter);
  
  void Update();
  void Render() noexcept;

private:

  // Get active(the first) Camera. Returns nullptr if there is no Camera.
  GameObject* GetCamera() noexcept;
  std::vector<RenderableMesh> GetMirrors() noexcept;
  std::vector<RenderableMesh> GetTransparents() noexcept;
  std::vector<const GameObject*> GetLights() noexcept;

  std::list<std::unique_ptr<GameObject>> objects_;
  bool first_update_ = true;

};

}  // namespace LL3D
