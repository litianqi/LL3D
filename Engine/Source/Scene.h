#pragma once

#include <memory>
#include <list>
#include "GameObject.h"
#include "Graphics/Base.h"

namespace LL3D {

class RecursiveSceneIterator;

class Scene : private Graphics::Base {
  friend RecursiveSceneIterator;

public:
  //--------------------------------------
  // GameObjects
 
  void AddGameObject(const GameObject& object);
  
  
  //--------------------------------------
  // Operations

  void Update();

private: 
  //>
  // Get main(the first) Camera. Returns nullptr if there is no Camera.
  //
  GameObject* GetCamera() noexcept;
  //>
  // RT.
  //
  std::vector<Component*> GetDeferRenderingModels() noexcept;

  std::list<GameObject> objects_;

  bool first_update_ = true;
};

}  // namespace LL3D
