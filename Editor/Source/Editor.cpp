#include "Editor.h"
#include <plog\Log.h>
#include <Window.h>
#include <GameObject.h>
#include <Scene.h>
#include <Component.h>
#include <Graphics\ModelRender.h>
#include <Graphics\Camera.h>
#include <Graphics\Device.h>
#include <Graphics\LightComponent.h>
#include <Graphics\Fog.h>
#include "PlayerController.h"
#include "WaveController.h"

using namespace std;
using namespace std::tr1;
using namespace LL3D;
using namespace DirectX;

Editor::Editor()
{  
  timer_.Start();
 
  // Add Camera.
  auto o0 = make_unique<GameObject>();
  const auto frustum = Graphics::Camera::Frustum(
    XM_PI / 8.0,
    static_cast<float>(window_->clientRect().GetSize().w) / window_->clientRect().GetSize().h,
    1,
    1000);
  auto c0 = make_unique<Graphics::Camera>(
    o0->transform(),
    frustum,
    XMVECTOR{ 0.0f, -100.0f, 100.0f });
  auto c00 = make_unique<EditorCameraController>(o0.get());
  auto c01 = make_unique<Graphics::Fog>(Math::Color(0.75f, 0.75f, 0.75f, 1.0f), 25.f, 1275.f);
  
  o0->addComponent(std::move(c0));
  o0->addComponent(std::move(c00));
  o0->transform().setPosition(XMVECTOR{ 0.0f, 100.0f, -100, 1.0f });
  //o0.AddComponent(std::move(c01));
  scene_->add(std::move(o0));

  // Add girl.
  auto mr1 = make_unique<Graphics::ModelRender>(
    "Resource\\Models\\girl\\girl.obj"
    );
  auto girl = make_unique<GameObject>();
  girl->addComponent(std::move(mr1));
  girl->transform().setScale(Math::Vector3(5.f, 5.f, 5.f));
  girl->transform().setRotation(Math::Vector3(Math::XM_PIDIV2, 0.f, 0.f));
  girl->setName("Model");
  scene_->add(std::move(girl));

  // Add cube.
  auto mr2 = make_unique<Graphics::ModelRender>(Graphics::ModelRender::Cube);
  auto cube = make_unique<GameObject>();
  cube->addComponent(std::move(mr2));
  cube->transform().setPosition(Math::Vector3(10.f, 5.f, 0.f));
  scene_->add(std::move(cube));

  // Add sphere.
  auto mr3 = make_unique<Graphics::ModelRender>(Graphics::ModelRender::Sphere);
  auto sphere = make_unique<GameObject>();
  sphere->addComponent(std::move(mr3));
  sphere->transform().setPosition(Math::Vector3(-10.f, 5.f, 0.f));
  scene_->add(std::move(sphere));

  // Add grid.
  auto mr4 = make_unique<Graphics::ModelRender>(Graphics::ModelRender::Grid);
  auto grid = make_unique<GameObject>();
  grid->addComponent(std::move(mr4));
  grid->setName("Earth");
  scene_->add(std::move(grid));

  // Add transparent.
  auto mesh0 = Graphics::Mesh::createGrid(90, 90, 2, 2);
  mesh0.materialIndex = 0;
  auto mat0 = Graphics::Material();
  mat0.diffuse = Math::Vector3(1.f, 1.f, 1.f);
  mat0.opacity = 0.45f;
  std::vector<Graphics::Mesh>     meshes0;
  std::vector<Graphics::Material> materials0;
  meshes0.push_back(mesh0);
  materials0.push_back(mat0);
  auto model0 = Graphics::Model("", move(meshes0), move(materials0));
  auto mr5 = make_unique<Graphics::ModelRender>(model0);
  auto transparent = make_unique<GameObject>();
  transparent->addComponent(std::move(mr5));
  transparent->transform().setPosition(Math::Vector3(0.f, 5.f, 0.f));
  scene_->add(std::move(transparent));

  // Add mirror.
  auto mesh1 = Graphics::Mesh::createGrid(90, 90, 2, 2);
  mesh1.materialIndex = 0;
  auto mat1 = Graphics::Material();
  mat1.diffuse = Math::Vector3(1.f, 1.f, 1.f);
  mat1.opacity = 0.15f;
  mat1.mirror = true;
  std::vector<Graphics::Mesh>     meshes1;
  std::vector<Graphics::Material> materials1;
  meshes1.push_back(mesh1);
  materials1.push_back(mat1);
  auto model1 = Graphics::Model("", move(meshes1), move(materials1));
  auto mr6 = make_unique<Graphics::ModelRender>(model1);
  auto mirror = make_unique<GameObject>();
  mirror->addComponent(std::move(mr6));
  mirror->transform().setRotation(
    Math::Vector3(-Math::XM_PIDIV2, 0.f, 0.f)
    );
  mirror->transform().setPosition(Math::Vector3(0.f, 45.f, 25.f));
  scene_->add(std::move(mirror));

  // Add mirror2.
  auto mr7 = make_unique<Graphics::ModelRender>(model1);
  auto mirror2 = make_unique<GameObject>();
  mirror2->addComponent(std::move(mr7));
  mirror2->transform().setRotation(
    Math::Vector3(0.f, 0.f, Math::XM_PIDIV2)
    );
  mirror2->transform().setPosition(Math::Vector3(45.f, 45.f, 0.f));
  //scene_->AddGameObject(mirror2);

  // Add castle.
  /*auto mr5 = make_unique<Graphics::ModelRender>(
    "Resource\\Models\\Castle Interior\\Castle Interior.3DS"
    );
  auto castle = make_unique<GameObject>();
  castle.AddComponent(std::move(mr5));
  castle.GetTransform().SetPosition(Math::Vector3(-30.f, 212.f, 0.f));
  scene_->AddGameObject(castle);*/

  // Add ambient light:
  auto ambient_light = make_unique<GameObject>();
  auto ambient_component = make_unique<Graphics::LightComponent>(
    ambient_light->transform(),
    Graphics::LightComponent::Ambient
  );
  ambient_light->addComponent(std::move(ambient_component));
  scene_->add(std::move(ambient_light));

  // Add directional light:
  auto directional_light = make_unique<GameObject>();
  auto directional_component = make_unique<Graphics::LightComponent>(
    directional_light->transform(),
    Graphics::LightComponent::Directional
    );
  directional_light->addComponent(std::move(directional_component));
  directional_light->transform().setRotation(
    Math::Vector3(Math::XM_PI - 0.1f, 0.f, 0.f)
    );
  scene_->add(std::move(directional_light));

  // Add point light:
  auto point_light = make_unique<GameObject>();
  auto point_component = make_unique<Graphics::LightComponent>(
    point_light->transform(),
    Graphics::LightComponent::Point
    );
  point_light->addComponent(std::move(point_component));
  point_light->transform().setPosition(Math::Vector3(-20.f, 5.f, 0.f));
  scene_->add(std::move(point_light));

  // Add spot light:
  auto spot_light = make_unique<GameObject>();
  auto spot_component = make_unique<Graphics::LightComponent>(
    spot_light->transform(),
    Graphics::LightComponent::Spot
    );
  spot_light->addComponent(std::move(spot_component));
  spot_light->transform().setPosition(Math::Vector3(22.f, 5.f, 0.f));
  spot_light->transform().setRotation(Math::Vector3(0.f, 0.f, Math::XM_PIDIV2));
  scene_->add(std::move(spot_light));

  window_->setVisible(true);
}

void Editor::update() {
}

void Editor::onResize() {
  graphicsDevice_->onResize(IntSize2{ window_->clientRect().GetSize().w, window_->clientRect().GetSize().h });
}