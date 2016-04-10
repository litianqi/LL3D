#pragma once

template<typename T>
inline void GameObject::addComponent() {
  components_[type_index(typeid(T))] = make_unique<T>();
}

template<typename T>
inline void GameObject::removeComponent() {
  components_.erase(std::type_index(typeid(T)));
}

template<typename T>
inline T * GameObject::component() {
  return const_cast<T*>(const_cast<const GameObject*>(this)->component<T>());
}

template <typename T>
inline const T* GameObject::component() const {
  auto search = components_.find(std::type_index(typeid(T)));
  if (search != components_.end())
    return dynamic_cast<T*>(search->second.get());
  else
    return nullptr;
}