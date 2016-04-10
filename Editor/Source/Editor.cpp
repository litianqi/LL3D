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
    static_cast<float>(window_->GetClientRect().GetSize().w) / window_->GetClientRect().GetSize().h,
    1,
    1000);
  auto c0 = make_unique<Graphics::Camera>(
    o0->GetTransform(),
    frustum,
    XMVECTOR{ 0.0f, -100.0f, 100.0f });
  auto c00 = make_unique<EditorCameraController>(o0.get());
  auto c01 = make_unique<Graphics::Fog>(Math::Color(0.75f, 0.75f, 0.75f, 1.0f), 25.f, 1275.f);
  
  o0->AddComponent(std::move(c0));
  o0->AddComponent(std::move(c00));
  o0->GetTransform().SetPosition(XMVECTOR{ 0.0f, 100.0f, -100, 1.0f });
  //o0.AddComponent(std::move(c01));
  scene_->Add(std::move(o0));

  // Add girl.
  auto mr1 = make_unique<Graphics::ModelRender>(
    "Resource\\Models\\girl\\girl.obj"
    );
  auto girl = make_unique<GameObject>();
  girl->AddComponent(std::move(mr1));
  girl->GetTransform().SetScale(Math::Vector3(5.f, 5.f, 5.f));
  girl->GetTransform().SetRotation(Math::Vector3(Math::XM_PIDIV2, 0.f, 0.f));
  girl->SetName("Model");
  scene_->Add(std::move(girl));

  // Add cube.
  auto mr2 = make_unique<Graphics::ModelRender>(Graphics::ModelRender::Cube);
  auto cube = make_unique<GameObject>();
  cube->AddComponent(std::move(mr2));
  cube->GetTransform().SetPosition(Math::Vector3(10.f, 5.f, 0.f));
  scene_->Add(std::move(cube));

  // Add sphere.
  auto mr3 = make_unique<Graphics::ModelRender>(Graphics::ModelRender::Sphere);
  auto sphere = make_unique<GameObject>();
  sphere->AddComponent(std::move(mr3));
  sphere->GetTransform().SetPosition(Math::Vector3(-10.f, 5.f, 0.f));
  scene_->Add(std::move(sphere));

  // Add grid.
  auto mr4 = make_unique<Graphics::ModelRender>(Graphics::ModelRender::Grid);
  auto grid = make_unique<GameObject>();
  grid->AddComponent(std::move(mr4));
  grid->SetName("Earth");
  scene_->Add(std::move(grid));

  // Add transparent.
  auto mesh0 = Graphics::Mesh::CreateGrid(90, 90, 2, 2);
  mesh0.material_index = 0;
  auto mat0 = Graphics::Material();
  mat0.diffuse = Math::Vector3(1.f, 1.f, 1.f);
  mat0.opacity = 0.45f;
  auto model0 = Graphics::Model();
  model0.meshes.push_back(mesh0);
  model0.materials.push_back(mat0);
  auto mr5 = make_unique<Graphics::ModelRender>(model0);
  auto transparent = make_unique<GameObject>();
  transparent->AddComponent(std::move(mr5));
  transparent->GetTransform().SetPosition(Math::Vector3(0.f, 5.f, 0.f));
  scene_->Add(std::move(transparent));

  // Add mirror.
  auto mesh1 = Graphics::Mesh::CreateGrid(90, 90, 2, 2);
  mesh1.material_index = 0;
  auto mat1 = Graphics::Material();
  mat1.diffuse = Math::Vector3(1.f, 1.f, 1.f);
  mat1.opacity = 0.15f;
  mat1.is_mirror = true;
  auto model1 = Graphics::Model();
  model1.meshes.push_back(mesh1);
  model1.materials.push_back(mat1);
  auto mr6 = make_unique<Graphics::ModelRender>(model1);
  auto mirror = make_unique<GameObject>();
  mirror->AddComponent(std::move(mr6));
  mirror->GetTransform().SetRotation(
    Math::Vector3(-Math::XM_PIDIV2, 0.f, 0.f)
    );
  mirror->GetTransform().SetPosition(Math::Vector3(0.f, 45.f, 25.f));
  scene_->Add(std::move(mirror));

  // Add mirror2.
  auto mr7 = make_unique<Graphics::ModelRender>(model1);
  auto mirror2 = make_unique<GameObject>();
  mirror2->AddComponent(std::move(mr7));
  mirror2->GetTransform().SetRotation(
    Math::Vector3(0.f, 0.f, Math::XM_PIDIV2)
    );
  mirror2->GetTransform().SetPosition(Math::Vector3(45.f, 45.f, 0.f));
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
    ambient_light->GetTransform(),
    Graphics::LightComponent::Ambient
  );
  ambient_light->AddComponent(std::move(ambient_component));
  scene_->Add(std::move(ambient_light));

  // Add directional light:
  auto directional_light = make_unique<GameObject>();
  auto directional_component = make_unique<Graphics::LightComponent>(
    directional_light->GetTransform(),
    Graphics::LightComponent::Directional
    );
  directional_light->AddComponent(std::move(directional_component));
  directional_light->GetTransform().SetRotation(
    Math::Vector3(Math::XM_PI - 0.1f, 0.f, 0.f)
    );
  scene_->Add(std::move(directional_light));

  // Add point light:
  auto point_light = make_unique<GameObject>();
  auto point_component = make_unique<Graphics::LightComponent>(
    point_light->GetTransform(),
    Graphics::LightComponent::Point
    );
  point_light->AddComponent(std::move(point_component));
  point_light->GetTransform().SetPosition(Math::Vector3(-20.f, 5.f, 0.f));
  scene_->Add(std::move(point_light));

  // Add spot light:
  auto spot_light = make_unique<GameObject>();
  auto spot_component = make_unique<Graphics::LightComponent>(
    spot_light->GetTransform(),
    Graphics::LightComponent::Spot
    );
  spot_light->AddComponent(std::move(spot_component));
  spot_light->GetTransform().SetPosition(Math::Vector3(22.f, 5.f, 0.f));
  spot_light->GetTransform().SetRotation(Math::Vector3(0.f, 0.f, Math::XM_PIDIV2));
  scene_->Add(std::move(spot_light));

  window_->SetVisible(true);
}

void Editor::Update() {
}

void Editor::OnResize() {
  graphics_device_->OnResize(IntSize2{ window_->GetClientRect().GetSize().w, window_->GetClientRect().GetSize().h });
}