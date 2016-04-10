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

  void add(std::unique_ptr<GameObject> object);
  void remove(RecursiveSceneIterator iter);
  
  void update();
  void render() noexcept;

private:

  // Get active(the first) Camera. Returns nullptr if there is no Camera.
  GameObject* camera() noexcept;
  std::vector<RenderableMesh> mirrors() noexcept;
  std::vector<RenderableMesh> transparents() noexcept;
  std::vector<const GameObject*> lights() noexcept;

  std::list<std::unique_ptr<GameObject>> objects_;
  bool first_update_ = true;

};

}  // namespace LL3D
