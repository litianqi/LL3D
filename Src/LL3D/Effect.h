#pragma once

#include <string>

namespace LL3D {

class Effect {
public:
  Effect();

  void Load(const std::string& file_name);
};

}