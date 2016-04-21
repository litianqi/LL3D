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
class Scene : private Graphics::Base
{
  friend RecursiveSceneIterator;

public:
  Scene(Window* window);

  void update();
  void render();

  void add(std::unique_ptr<GameObject> object);
  void remove(RecursiveSceneIterator iter);

  // Gets active(the first) Camera. Returns nullptr if there is no Camera.
  const GameObject* camera() const;

  // Gets GameObject user is picking at in this frame.
  // Returns nullptr if user isn't picking.
  const GameObject* picking() const;

private:
  std::vector<RenderableMesh> mirrors();
  std::vector<RenderableMesh> transparents();
  std::vector<const GameObject*> lights();

  void retrieveCamera();
  void calculatePicking();

  std::list<std::unique_ptr<GameObject>> objects_;
  Window* window_ = nullptr;
  bool first_update_ = true;

  // BEGIN Cached values, calculate/retrieve once a frame.
  const GameObject* camera_ = nullptr;
  const GameObject* picking_ = nullptr;
  // END
};

} // namespace LL3D
