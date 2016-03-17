#pragma once

#include <memory>
#include <list>
#include "GameObject.h"
#include "Graphics/Base.h"

namespace LL3D {

class Scene : private Graphics::Base {
public:
  void Update();

  void AddGameObject(const GameObject& object);
   
private: 
  std::list<GameObject> objects_;
};

}  // namespace LL3D
