#pragma once

#include <memory>
#include <list>
#include "GameObject.h"

namespace LL3D {

class Scene {
public:
  Scene() {}

private:
  std::list<std::shared_ptr<GameObject>> objects_;
};

}  // namespace LL3D
