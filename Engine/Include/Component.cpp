#include "Component.h"

using namespace std;

namespace LL3D {

void Component::SetGameObject(GameObject * value) {
  game_object_ = value;
}

GameObject* Component::GetGameObject() {
  return game_object_;
}

const GameObject * Component::GetGameObject() const {
  return game_object_;
}

std::string Component::GetType() const {
  auto full_name = std::string(typeid(*this).name());
  auto pos = full_name.find_last_of(':');
  return full_name.substr(pos + 1);
}

}  // namespace LL3D 
