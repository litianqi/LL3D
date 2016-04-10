#pragma once

#include "Component.h"

namespace LL3D {

class Behaviour : public Component {
public:
  Behaviour(GameObject* object) : object_(object) {}

protected:
  const GameObject* object() const { return object_; }
  GameObject* object() { return object_; }

private:
  GameObject* object_;
};

}  // namespace LL3D