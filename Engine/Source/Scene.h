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

class Window;
class RecursiveSceneIterator;
using RenderableMesh = std::pair<Transform, const Graphics::MeshRender*>;

}


namespace LL3D {

class Scene : private Graphics::Base {

  friend RecursiveSceneIterator;

public: 
  Scene(Window* window);

  void add(std::unique_ptr<GameObject> object);
  void remove(RecursiveSceneIterator iter);

  const GameObject* picking() const;
  
  void update();
  void render() noexcept;

private:
  // Get active(the first) Camera. Returns nullptr if there is no Camera.
  GameObject* camera() noexcept;
  std::vector<RenderableMesh> mirrors() noexcept;
  std::vector<RenderableMesh> transparents() noexcept;
  std::vector<const GameObject*> lights() noexcept;
  void calculatePicking();

  std::list<std::unique_ptr<GameObject>> objects_;
  Window* window_ = nullptr;
  bool first_update_ = true;

  // Cached values, calculated once per frame.
  // Which GameObject user is picking on, nullptr if no picking.
  const GameObject* picking_ = nullptr;
};

}  // namespace LL3D
