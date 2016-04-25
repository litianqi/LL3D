#pragma once

#include <string>
#include <memory>
#include <chrono>
#include "Core/Uncopyable.h"

namespace LL3D {
class GameObject;
}

namespace LL3D {

class Component
{
public:
  virtual ~Component() = default;

  virtual void start() {}
  void tryStart()
  {
    if (nonStarted) {
      start();
      nonStarted = false;
    }
  }

  virtual void update() {}
  virtual void render() {}

private:
  bool nonStarted = true;
};

} // namespace LL3D
