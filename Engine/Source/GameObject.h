#pragma once

#include <list>
#include <unordered_map>
#include <typeindex>
#include <memory>
#include "Transform.h"

namespace LL3D {

class Scene;
class Component;

///
// Base class for all entities in Unity scenes.
//
class GameObject {
public:          

  GameObject();
  GameObject(const std::string& name) noexcept;
  GameObject(const GameObject& other) noexcept;
  GameObject& operator=(const GameObject& other) noexcept;
  GameObject(GameObject&&) = default;
  GameObject& operator=(GameObject&& other) = default;
  ~GameObject() = default;

  //////////////////////////////////////////////////////////////////////////
  /////////////////////////////// Relations

  void SetScene(Scene* scene);
  void SetParent(GameObject* parent);
  void AddChild(GameObject child);
  void RemoveChild(GameObject* child);

  Scene* GetScene();
  const Scene* GetScene() const;
  GameObject* GetParent();
  const GameObject* GetParent() const;

  //////////////////////////////////////////////////////////////////////////
  /////////////////////////////// Properties
   
  void SetActive(bool value);
  void SetName(const std::string& value);
  void SetTag(const std::string& value);
  void AddComponent(std::unique_ptr<Component> component);
  template<typename T> 
  void AddComponent();
  template<typename T>
  void RemoveComponent();

  bool GetActive() const;
  bool GetActiveInHierarchy() const;
  const std::string& GetName() const;
  const std::string& GetTag() const;
  template <typename T> 
  T* GetComponent();
  template <typename T>
  const T* GetComponent() const;
  
  //////////////////////////////////////////////////////////////////////////
  /////////////////////////////// Operations
  
  ///
  // Updates its components. Called by it's parent or scene.
  //
  void Update();

private:
  using ComponentMap = std::unordered_map<std::type_index, 
    std::unique_ptr<Component>>;

  GameObject*           parent_ = nullptr;
  std::list<GameObject> children_;
  Scene*                scene_ = nullptr;

  bool          active_;
  std::string   name_;
  std::string   tag_;
  ComponentMap  components_;
};

}  // namespace LL3D

#include "GameObject.imp"