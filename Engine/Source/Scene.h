#pragma once

#include <memory>
#include <list>
#include "GameObject.h"
#include "Graphics/Base.h"
#include "Graphics/ModelRender.h"

namespace LL3D {

class RecursiveSceneIterator;

class Scene : private Graphics::Base {
  friend RecursiveSceneIterator;

public: 
  void AddGameObject(const GameObject& object);
  void Update();

private: 
  //>
  // Get main(the first) Camera. Returns nullptr if there is no Camera.
  //
  GameObject* GetCamera() noexcept;
  std::vector<std::pair<Transform, Graphics::MeshRender>> meshes_;   
  std::vector<GameObject*> GetTransparentGameObjects() noexcept;
  std::vector<Component*> GetMirrorModelRenders() noexcept;

  std::list<GameObject> objects_;
  bool first_update_ = true;
};

}  // namespace LL3D
