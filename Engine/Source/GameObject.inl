#pragma once

template<typename T>
inline void GameObject::AddComponent() {
  components_[type_index(typeid(T))] = make_unique<T>();
  components_[type_index(typeid(T))]->SetGameObject(this);
}

template<typename T>
inline void GameObject::RemoveComponent() {
  components_.erase(std::type_index(typeid(T)));
}

template<typename T>
inline T * GameObject::GetComponent() {
  return const_cast<T*>(const_cast<const GameObject*>(this)->GetComponent<T>());
}

template <typename T>
inline const T* GameObject::GetComponent() const {
  auto search = components_.find(std::type_index(typeid(T)));
  if (search != components_.end())
    return dynamic_cast<T*>(search->second.get());
  else
    return nullptr;
}