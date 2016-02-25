#pragma once

#include <chrono>
#include "Window.h"

namespace LL3D {

class Game : public Window {
public:
  void Run();

protected:
  virtual void Update() = 0;
  virtual void Render() = 0;
};

}  // namespace LL3D