#pragma once

#include <memory>
#include <list>
#include "GameObject.h"
#include "Graphics/Base.h"
#include "Graphics/ModelRender.h"

namespace LL3D {

class RecursiveSceneIterator;
using RenderableMesh = std::pair<Transform, const Graphics::MeshRender*>;

class Scene : private Graphics::Base {
  friend RecursiveSceneIterator;

public: 
  void AddGameObject(const GameObject& object);
  void Update();
  void Render() noexcept;

  //>
  // Get main(the first) Camera. Returns nullptr if there is no Camera.
  //
  GameObject* GetCamera() noexcept;

private:   
  std::vector<RenderableMesh> GetMirrors() noexcept;
  std::vector<RenderableMesh> GetTransparents() noexcept;

  std::list<GameObject> objects_;
  bool first_update_ = true;
};

}  // namespace LL3D
