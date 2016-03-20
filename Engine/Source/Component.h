#pragma once

#include <string>
#include <memory>

namespace LL3D {

class GameObject;

class Component {
public:
  virtual std::unique_ptr<Component> Clone() = 0;
  virtual ~Component() = default;
  
  
  //--------------------------------------
  // Relations

  //>
  // Set GameObject this Componet belongs to. Generally called by GameObeject.
  //
  void SetGameObject(GameObject* value);

  GameObject*       GetGameObject();
  const GameObject* GetGameObject() const;

  
  //--------------------------------------
  // Queries

  //>
  // Returns a string reprensents real type of this. (e.g. "Model")
  //
  std::string GetType() const;

  
  //--------------------------------------
  // Operations

  virtual void Start() {}
  virtual void Update() {}

private:
  GameObject* game_object_ = nullptr;
};

}  // namespace LL3D
