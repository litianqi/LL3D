#include "GameObject.h"
#include "Graphics/Model.h"
#include "Graphics/ModelRender.h"
#include "Core/Assert.h"
#include "Component.h"
#include "Transform.h"

using namespace std;

namespace LL3D {

GameObject::GameObject() {
  auto transform = make_unique<Transform>(nullptr);
  AddComponent(std::move(transform));
}

GameObject::GameObject(const std::string & name) noexcept :
name_(name) 
{
  auto transform = make_unique<Transform>(nullptr);
  AddComponent(std::move(transform));
}

void GameObject::Start()
{
  for (auto& child : children_) {
    child->Start();
  }

  for (auto& component : components_) {
    component.second->Start();
  }
}

void GameObject::Update() {
  for (auto& child : children_) {
    child->Update();
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
  const auto& parentTransform = parent->GetTransform();
  GetTransform().setParentTransform(&parentTransform);
}

void GameObject::AddChild(std::unique_ptr<GameObject> child) {
  child->SetParent(this);
  children_.push_back(std::move(child));
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
