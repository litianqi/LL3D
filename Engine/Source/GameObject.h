#pragma once

#include <list>
#include <memory>
#include "Component.h"
#include "Transform.h"

namespace LL3D {

class GameObject {
public:
  GameObject() {}

private:
  GameObject*                             parent_;
  std::list<std::unique_ptr<GameObject>>  children_;

  std::string                             tag_;
  Transform                               transform_;
  std::list<std::unique_ptr<Component>>   components_;
};

}  // namespace LL3D