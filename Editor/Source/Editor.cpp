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
  auto c0 = make_unique<Graphics::Camera>(Graphics::Camera::Frustum(
    XM_PI / 8.0,
    static_cast<float>(window_->GetClientRect().GetSize().w) / window_->GetClientRect().GetSize().h,
    1,
    1000),
    XMVECTOR{ 0.0f, -100.0f, 100.0f });
  auto c00 = make_unique<EditorCameraController>();
  auto c01 = make_unique<Graphics::Fog>(Math::Color(0.75f, 0.75f, 0.75f, 1.0f), 25.f, 1275.f);
  auto o0 = GameObject();
  o0.AddComponent(std::move(c0));
  o0.AddComponent(std::move(c00));
  o0.GetComponent<Transform>()->SetPosition(XMVECTOR{ 0.0f, 100.0f, -100, 1.0f });
  //o0.AddComponent(std::move(c01));
  scene_->AddGameObject(o0);

  // Add girl.
  auto mr1 = make_unique<Graphics::ModelRender>(
    "D:\\Workspace\\LL3D\\Editor\\Resource\\Models\\girl\\girl.obj"
    );
  auto girl = GameObject();
  girl.AddComponent(std::move(mr1));
  girl.GetComponent<Transform>()->SetScale(Math::Vector3(5.f, 5.f, 5.f));
  girl.GetComponent<Transform>()->SetRotation(Math::Vector3(Math::XM_PIDIV2, 0.f, 0.f));
  girl.SetName("Model");
  scene_->AddGameObject(girl);

  // Add cube.
  auto mr2 = make_unique<Graphics::ModelRender>(Graphics::ModelRender::Cube);
  auto cube = GameObject();
  cube.AddComponent(std::move(mr2));
  cube.GetComponent<Transform>()->SetPosition(Math::Vector3(10.f, 5.f, 0.f));
  scene_->AddGameObject(cube);

  // Add sphere.
  auto mr3 = make_unique<Graphics::ModelRender>(Graphics::ModelRender::Sphere);
  auto sphere = GameObject();
  sphere.AddComponent(std::move(mr3));
  sphere.GetComponent<Transform>()->SetPosition(Math::Vector3(-10.f, 5.f, 0.f));
  scene_->AddGameObject(sphere);

  // Add grid.
  auto mr4 = make_unique<Graphics::ModelRender>(Graphics::ModelRender::Grid);
  auto grid = GameObject();
  grid.AddComponent(std::move(mr4));
  scene_->AddGameObject(grid);

  // Add castle.
  /*auto mr5 = make_unique<Graphics::ModelRender>(
    "D:\\Workspace\\LL3D\\Editor\\Resource\\Models\\Castle Interior\\Castle Interior.3DS"
    );
  auto castle = GameObject();
  castle.AddComponent(std::move(mr5));
  castle.GetComponent<Transform>()->SetPosition(Math::Vector3(-30.f, 212.f, 0.f));
  scene_->AddGameObject(castle);*/

  // Add ambient light:
  auto ambient_component = make_unique<Graphics::LightComponent>(
    Graphics::LightComponent::Ambient
  );
  auto ambient_light = GameObject();
  ambient_light.AddComponent(std::move(ambient_component));
  scene_->AddGameObject(ambient_light);

  // Add directional light:
  auto directional_component = make_unique<Graphics::LightComponent>(
    Graphics::LightComponent::Directional
    );
  auto directional_light = GameObject();
  directional_light.AddComponent(std::move(directional_component));
  directional_light.GetComponent<Transform>()->SetRotation(Math::Vector3(Math::XM_PIDIV2 - 0.1f, 0.f, 0.f));
  scene_->AddGameObject(directional_light);

  // Add point light:
  auto point_component = make_unique<Graphics::LightComponent>(
    Graphics::LightComponent::Point
    );
  auto point_light = GameObject();
  point_light.AddComponent(std::move(point_component));
  point_light.GetComponent<Transform>()->SetPosition(Math::Vector3(-20.f, 5.f, 0.f));
  scene_->AddGameObject(point_light);

  // Add spot light:
  auto spot_component = make_unique<Graphics::LightComponent>(
    Graphics::LightComponent::Spot
    );
  auto spot_light = GameObject();
  spot_light.AddComponent(std::move(spot_component));
  spot_light.GetComponent<Transform>()->SetPosition(Math::Vector3(22.f, 5.f, 0.f));
  spot_light.GetComponent<Transform>()->SetRotation(Math::Vector3(0.f, -Math::XM_PIDIV2, 0.f));
  scene_->AddGameObject(spot_light);

  window_->SetVisible(true);
}

void Editor::Update() {
}

void Editor::OnResize() {
  graphics_device_->OnResize(IntSize2{ window_->GetClientRect().GetSize().w, window_->GetClientRect().GetSize().h });
  scene_->Update();  // TODO: condider delete this line, only draw in main loop.
}