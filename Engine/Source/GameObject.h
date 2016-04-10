#pragma once

#include <list>
#include <unordered_map>
#include <typeindex>
#include <memory>
#include "Core/Uncopyable.h"

namespace LL3D {

class Scene;
class Component;
class RecursiveSceneIterator;
class Transform;

}


namespace LL3D {

// Base class for all entities in LL3D scenes.
class GameObject : private Core::Uncopyable {

  friend RecursiveSceneIterator;

public:          

  GameObject();
  GameObject(const std::string& name) noexcept;
  GameObject(GameObject&&) = default;
  GameObject& operator=(GameObject&& other) = default;
  ~GameObject() = default;
  
  void setScene(Scene* scene);
  void setParent(GameObject* parent);
  void addChild(std::unique_ptr<GameObject> child);
  void removeChild(GameObject* child);

  Scene* scene();
  const Scene* scene() const;
  GameObject* parent();
  const GameObject* parent() const;
     
  void setActive(bool value);
  void setName(const std::string& value);
  void setTag(const std::string& value);
  void addComponent(std::unique_ptr<Component> component);
  template<typename T> 
  void addComponent();
  template<typename T>
  void removeComponent();

  bool active() const;
  bool activeInHierarchy() const;
  const std::string& name() const;
  const std::string& tag() const;
  template <typename T> 
  T* component();
  template <typename T>
  const T* component() const;
  Transform& transform();
  const Transform& transform() const;
  
  void start();
  // Updates its components and children. Called by it's parent or Scene.
  void update();

private:

  using ComponentMap = std::unordered_map<std::type_index, 
    std::unique_ptr<Component>>;

  GameObject*           parent_ = nullptr;
  std::list<std::unique_ptr<GameObject>> children_;
  Scene*                scene_ = nullptr;
  
  bool          active_;
  std::string   name_;
  std::string   tag_;
  ComponentMap  components_;

};

#include "GameObject.inl"

}  // namespace LL3D

