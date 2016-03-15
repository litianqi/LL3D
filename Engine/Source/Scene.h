#pragma once

#include <memory>
#include <list>
#include "GameObject.h"
#include "Graphics/Base.h"

namespace LL3D {

//namespace Graphics {
//class Camera;
//}

class Scene : private Graphics::Base {
public:
  void Update();

  void AddGameObject(const GameObject& object);
   
private: 
  std::list<GameObject> objects_;

  // Cached working camera.
  //Graphics::Camera* camera;
};

}  // namespace LL3D
