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
  addComponent(std::move(transform));
}

GameObject::GameObject(const std::string & name) noexcept :
name_(name) 
{
  auto transform = make_unique<Transform>(nullptr);
  addComponent(std::move(transform));
}

void GameObject::start()
{
  for (auto& child : children_) {
    child->start();
  }

  for (auto& component : components_) {
    component.second->start();
  }
}

void GameObject::update() {
  for (auto& child : children_) {
    child->update();
  }

  for (auto& component : components_) {
    component.second->update();
  }

  for (const auto& component : components_) {
    component.second->update();
  }
}

Scene * GameObject::scene() {
  return scene_;
}

void GameObject::setScene(Scene * scene) {
  scene_ = scene;
}

void GameObject::setParent(GameObject * parent) {
  parent_ = parent;
  const auto& parentTransform = parent->transform();
  transform().setParentTransform(&parentTransform);
}

void GameObject::addChild(std::unique_ptr<GameObject> child) {
  child->setParent(this);
  children_.push_back(std::move(child));
}

void GameObject::addComponent(std::unique_ptr<Component> component) {
  components_[type_index(typeid(*component))] = std::move(component);
}

void GameObject::setName(const std::string & value) {
  name_ = value;
}

void GameObject::setTag(const std::string & value) {
  tag_ = value;
}

const std::string & GameObject::name() const {
  return name_;
}

const std::string& GameObject::tag() const {
  return tag_;
}

Transform & GameObject::transform()
{
  return const_cast<Transform&>(const_cast<const GameObject*>(this)->transform());
}

const Transform & GameObject::transform() const
{
  auto transform = component<Transform>();
  ASSERT(transform);
  return *transform;
}

GameObject * GameObject::parent() {
  return parent_;
}

const GameObject * GameObject::parent() const {
  return parent_;
}

}  // namespace LL3D
