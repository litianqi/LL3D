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
#include "WaveController.h"
#include "PlayerController.h"
#include "SkyboxController.h"

using namespace std;
using namespace std::tr1;
using namespace LL3D;
using namespace DirectX;

Editor::Editor()
{
  timer_.start();

  // Add player.
  auto player = make_unique<GameObject>();
  auto camera = make_unique<Graphics::Camera>(
    player->transform()
    );
  const auto aspectRation = static_cast<float>(window_->clientRect().GetSize().w) /
    window_->clientRect().GetSize().h;
  camera->setAspectRatio(aspectRation);
  player->addComponent(std::move(camera));

  auto controller = make_unique<PlayerController>();
  controller->setObject(player.get());
  player->addComponent(std::move(controller));

  player->transform().setPosition(
    Math::Vector3(0.0f, 10.0f, -100)
  );
  player->setName("player");
  //camera_object->transform().setRotation();
  scene_->add(std::move(player));

  // Add girl.
  auto mr1 = make_unique<Graphics::ModelRender>(
    "Resource\\Models\\girl\\girl.obj"
    );
  auto girl = make_unique<GameObject>();
  girl->addComponent(std::move(mr1));
  girl->transform().setScale(Math::Vector3(5.f, 5.f, 5.f));
  girl->transform().setRotation(Math::Vector3(Math::XM_PIDIV2, 0.f, 0.f));
  girl->setName("girl");
  scene_->add(std::move(girl));

  // Add cube.
  auto mr2 = make_unique<Graphics::ModelRender>(Graphics::ModelRender::kCube);
  mr2->begin()->material().diffuseTexture = "Resource/Textures/stones.dds";
  mr2->begin()->material().normalTexture = "Resource/Textures/stones_nmap.dds";
  auto cube = make_unique<GameObject>();
  cube->addComponent(std::move(mr2));
  cube->transform().setPosition(Math::Vector3(10.f, 5.f, 0.f));
  cube->setName("cube");
  scene_->add(std::move(cube));

  //for (auto i = 2; i < 12; i++) {
  //  for (auto j = 2; j < 12; j++) {
  //    auto mr2 = make_unique<Graphics::ModelRender>(Graphics::ModelRender::Cube);
  //    auto cube = make_unique<GameObject>();
  //    cube->addComponent(std::move(mr2));
  //    cube->transform().setPosition(Math::Vector3(20.f * i, 20.f * j, 0.f));
  //    scene_->add(std::move(cube));
  //  }
  //}

  // Add sphere.
  auto mr3 = make_unique<Graphics::ModelRender>(Graphics::ModelRender::kSphere);
  auto sphere = make_unique<GameObject>();
  sphere->addComponent(std::move(mr3));
  sphere->transform().setPosition(Math::Vector3(-10.f, 5.f, 0.f));
  sphere->setName("sphere");
  scene_->add(std::move(sphere));

  // Add cylinder.
  auto mrcy = make_unique<Graphics::ModelRender>(Graphics::ModelRender::kCylinder);
  mrcy->begin()->material().diffuseTexture = "Resource/Textures/bricks.dds";
  mrcy->begin()->material().normalTexture = "Resource/Textures/bricks_nmap.dds";
  auto cylinder = make_unique<GameObject>();
  cylinder->addComponent(std::move(mrcy));
  cylinder->transform().setPosition(Math::Vector3(-10.f, 15.f, 20.f));
  cylinder->setName("cylinder");
  scene_->add(std::move(cylinder));

  // Add grid.
  auto mr4 = make_unique<Graphics::ModelRender>(Graphics::ModelRender::kGrid);
  mr4->begin()->material().texTransform = Math::Matrix::CreateScale(
    Math::Vector3(10.f, 10.f, 0.f)
  );
  mr4->begin()->material().diffuseTexture = "Resource/Textures/floor.dds";
  mr4->begin()->material().normalTexture = "Resource/Textures/floor_nmap.dds";
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
  transparent->setName("transparent");
  scene_->add(std::move(transparent));

  // Add sky box.
  auto skybox = make_unique<GameObject>();

  auto meshSB = Graphics::Mesh::createSphere(900, 20, 20);
  meshSB.materialIndex = 0;
  std::vector<Graphics::Mesh>     meshesSB;
  meshesSB.push_back(meshSB);

  auto matSB = Graphics::Material();
  matSB.diffuseTexture = "Resource\\Textures\\sunsetcube1024.dds";
  std::vector<Graphics::Material> materialsSB;
  materialsSB.push_back(matSB);

  auto modelSB = Graphics::Model("", move(meshesSB), move(materialsSB));
  auto mrSB = make_unique<Graphics::ModelRender>(modelSB);
  mrSB->begin()[0].setCastShadow(false);
  skybox->addComponent(std::move(mrSB));

  auto sbCtroller = make_unique<SkyboxController>();
  sbCtroller->setObject(skybox.get());
  skybox->addComponent(std::move(sbCtroller));

  //skyBox->transform().setScale(Math::Vector3(100.f, 100.f, 100.f));
  skybox->setName("skyBox");
  scene_->add(std::move(skybox));

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
  mirror->setName("mirror");
  //scene_->add(std::move(mirror));

  // Add mirror2.
  auto mr7 = make_unique<Graphics::ModelRender>(model1);
  auto mirror2 = make_unique<GameObject>();
  mirror2->addComponent(std::move(mr7));
  mirror2->transform().setRotation(
    Math::Vector3(0.f, 0.f, Math::XM_PIDIV2)
  );
  mirror2->transform().setPosition(Math::Vector3(45.f, 45.f, 0.f));
  mirror2->setName("mirror2");
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
  ambient_light->setName("ambient");
  scene_->add(std::move(ambient_light));

  // Add directional light:
  auto directional_light = make_unique<GameObject>();
  auto directional_component = make_unique<Graphics::LightComponent>(
    directional_light->transform(),
    Graphics::LightComponent::Directional
    );
  directional_light->addComponent(std::move(directional_component));
  directional_light->transform().setRotation(
    Math::Vector3(Math::kPi - 0.1f, 0.f, 0.f)
  );
  directional_light->setName("directional");
  scene_->add(std::move(directional_light));

  // Add point light:
  auto point_light = make_unique<GameObject>();
  auto point_component = make_unique<Graphics::LightComponent>(
    point_light->transform(),
    Graphics::LightComponent::Point
    );
  point_light->addComponent(std::move(point_component));
  point_light->transform().setPosition(Math::Vector3(-20.f, 5.f, 0.f));
  point_light->setName("point");
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
  spot_light->setName("spot");
  scene_->add(std::move(spot_light));

  window_->setVisible(true);
}

void Editor::update()
{}

void Editor::onResize()
{
  graphicsDevice_->onResize(IntSize2{ window_->clientRect().GetSize().w, window_->clientRect().GetSize().h });
}