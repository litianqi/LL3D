#pragma once

#include <string>
#include <memory>
#include "Core/Uncopyable.h"

namespace LL3D {
class GameObject;
}

namespace LL3D {

class Component {
public:

  virtual ~Component() = default;
 
  virtual void Start() {}
  virtual void Update() {}

};

}  // namespace LL3D
