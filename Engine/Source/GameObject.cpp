#include "GameObject.h"
#include "Graphics/Model.h"
#include "Graphics/ModelRender.h"
#include "Core/Assert.h"
#include "Component.h"
#include "Transform.h"

using namespace std;

namespace LL3D {

GameObject::GameObject() {
  AddComponent<Transform>();
}

GameObject::GameObject(const std::string & name) noexcept :
name_(name) 
{
  AddComponent<Transform>();
}

GameObject::GameObject(const GameObject & other) noexcept :
parent_(other.parent_),
children_(other.children_),
scene_(other.scene_),
active_(other.active_),
name_(other.name_),
tag_(other.tag_)
{
  for (auto& child : children_) {
    child.SetParent(this);
  }

  for (const auto& component : other.components_) {
    components_[component.first] = std::move(component.second->Clone());
    components_[component.first]->SetGameObject(this);
  }
}

GameObject& GameObject::operator=(const GameObject& other) noexcept
{
  GameObject tmp(other);
  swap(tmp, *this);
  return *this;
}

void GameObject::Start()
{
  for (auto& child : children_) {
    child.Start();
  }

  for (auto& component : components_) {
    component.second->Start();
  }
}

void GameObject::Update() {
  for (auto& child : children_) {
    child.Update();
  }

  for (auto& component : components_) {
    component.second->Update();
  }

  for (const auto& component : components_) {
    component.second->Update();
  }
}

Scene * GameObject::GetScene() {
  return scene_;
}

void GameObject::SetScene(Scene * scene) {
  scene_ = scene;
}

void GameObject::SetParent(GameObject * parent) {
  parent_ = parent;
}

void GameObject::AddChild(GameObject child) {
  child.SetParent(this);
  children_.push_back(child);
}

void GameObject::AddComponent(std::unique_ptr<Component> component) {
  component->SetGameObject(this);
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

Transform & GameObject::GetTransform()
{
  return const_cast<Transform&>(const_cast<const GameObject*>(this)->GetTransform());
}

const Transform & GameObject::GetTransform() const
{
  auto transform = GetComponent<Transform>();
  ASSERT(transform);
  return *transform;
}

GameObject * GameObject::GetParent() {
  return parent_;
}

const GameObject * GameObject::GetParent() const {
  return parent_;
}

}  // namespace LL3D
