#pragma once

#include <string>
#include <memory>

namespace LL3D {

class Component {
public:
  virtual std::unique_ptr<Component> Clone() = 0;
  virtual ~Component() = default;

  virtual void Update() {}

  //GameObject* GetGameObject() const;
  std::string GetTypeName() const;
};

}  // namespace LL3D
