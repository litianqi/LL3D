#pragma once

#include <list>
#include <unordered_map>
#include <typeindex>
#include <memory>
#include "Transform.h"

namespace LL3D {

class Scene;
class Component;

class GameObject {
public:
  GameObject() {}
  GameObject(const std::string& name) noexcept;
  GameObject(const GameObject& other) noexcept;
  GameObject& operator=(const GameObject& other) noexcept;
  GameObject(GameObject&&) = default;
  GameObject& operator=(GameObject&& other) = default;
  ~GameObject() = default;

  void Update();

  Scene* GetScene();
  Transform& GetTransform();

  void SetActive(bool value);
  void SetName(const std::string& value);
  void SetTag(const std::string& value);

  void AddChild(const GameObject& object);
  void AddComponent(std::unique_ptr<Component> component);
  template<typename T>
  void AddComponent();

  bool GetActive() const;
  bool GetActiveInHierarchy() const;
  const std::string& GetName() const;
  const std::string& GetTag() const;
  
  template <typename T> 
  Component* GetComponent() const;

private:
  using ComponentMap = std::unordered_map<std::type_index, std::unique_ptr<Component>>;

  GameObject*           parent_ = nullptr;
  std::list<GameObject> children_;

  bool          active_;
  std::string   name_;
  std::string   tag_;
  Transform     transform_;
  ComponentMap  components_;
};

template<typename T>
inline void GameObject::AddComponent() {
  components_[type_index(typeid(T))] = make_unique<T>(dynamic_cast<const T&>());
}

template<typename T>
inline Component * GameObject::GetComponent() const {
  auto search = components_.find(std::type_index(typeid(T)));
  if (search != components_.end())
    return search->second.get();
  else
    return nullptr;
}

}  // namespace LL3D