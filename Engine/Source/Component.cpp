#include "Component.h"

#include <algorithm>

using namespace std;

namespace LL3D {

std::string Component::GetTypeName() const {
  auto full_name = std::string(typeid(*this).name());
  auto pos = full_name.find_last_of(':');
  return full_name.substr(pos + 1);
}

}  // namespace LL3D 
