#pragma once

#include <chrono>
#include "Window.h"

namespace LL3D {

class Application : public Window {
public:
  void Run();

protected:
  virtual void Update() = 0;  
};

}  // namespace LL3D