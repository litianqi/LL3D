#pragma once

#include <string>
#include <memory>

namespace LL3D {

class GameObject;

class Component {
public:
  virtual std::unique_ptr<Component> Clone() = 0;
  virtual ~Component() = default;
 
  //>
  // Set GameObject this Componet belongs to. Generally called by GameObeject.
  //
  void SetGameObject(GameObject* value);

  GameObject*       GetGameObject();
  const GameObject* GetGameObject() const;

  //>
  // Returns a string reprensents real type of this. (e.g. "ModelRender")
  //
  std::string GetType() const;

  virtual void Start() {}
  virtual void Update() {}

private:
  GameObject* game_object_ = nullptr;
};

}  // namespace LL3D
