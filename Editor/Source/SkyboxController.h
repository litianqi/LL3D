#pragma once

#include <Behaviour.h>
#include <GameObject.h>
#include <Transform.h>
#include <RecursiveSceneIterator.h>

class SkyboxController : public LL3D::Behaviour
{
public:
  void start() override
  {
    using namespace LL3D;

    myTransform_ = &(object()->transform());
    Scene* scene = object()->scene();
    for (auto& obj : RecursiveSceneIterator(*scene))
    {
      if (obj.name() == "player")
      {
        playerTransform_ = &(obj.transform());
      }
    }
  }

  // Skybox always follow player.
  void update() override
  {
    using namespace LL3D;

    myTransform_->setPosition(playerTransform_->position());
  }

private:
  LL3D::Transform* myTransform_;
  const LL3D::Transform* playerTransform_;
};
