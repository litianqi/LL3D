#pragma once

#include "Component.h"

namespace LL3D {

class Behaviour : public Component {
public:
  void setObject(GameObject* object) { object_ = object; }
  const GameObject* object() const { return object_; }
  GameObject* object() { return object_; }

private:
  GameObject* object_ = nullptr;
};

}  // namespace LL3D