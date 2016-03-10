#include "GameObject.h"
#include "Component.h"

using namespace std;

namespace LL3D {

GameObject::GameObject(const std::string & name) noexcept :
name_(name) 
{
}

GameObject::GameObject(const GameObject & other) noexcept :
active_(other.active_),
name_(other.name_),
tag_(other.tag_),
transform_(other.transform_)
{
  for (const auto& component : other.components_) {
    components_[component.first] = std::move(component.second->Clone());
  }
}

GameObject& GameObject::operator=(const GameObject& other) noexcept
{
  GameObject tmp(other);
  swap(tmp, *this);
  return *this;
}

void GameObject::Update() {
  for (auto& child : children_) {
    child.Update();
  }

  for (auto& component : components_) {
    component.second->Update();
  }
}

void GameObject::AddChild(const GameObject& object) {
  children_.push_back(object);
}

void GameObject::AddComponent(std::unique_ptr<Component> component) {
  components_[type_index(typeid(*component))] = std::move(component);
}

void GameObject::SetName(const std::string & value) {
  name_ = value;
}

void GameObject::SetTag(const std::string & value) {
  tag_ = value;
}

const std::string & GameObject::GetName() const {
  return name_;
}

const std::string& GameObject::GetTag() const {
  return tag_;
}

}  // namespace LL3D
