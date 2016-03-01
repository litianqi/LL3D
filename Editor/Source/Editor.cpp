#include "Editor.h"
#include <plog\Log.h>
#include "UIEvents.h"

using namespace LL3D;

Editor::Editor():
  first_personal_camera_(
    Camera::Frustum(
      XM_PI / 8.0, 
      static_cast<float>(GetClientRect().GetSize().w) / GetClientRect().GetSize().h, 
      1, 
      1000),
    XMVECTOR{ 0.0f, 200.0f, 0, 1.0f }, 
    XMVECTOR{ 0.0001f, -1.0f, 0.0001f }),
  editor_camera_(
    Camera::Frustum(
      XM_PI / 8.0,
      static_cast<float>(GetClientRect().GetSize().w) / GetClientRect().GetSize().h,
      1,
      1000),
    XMVECTOR{ 0.0f, 100.0f, -100, 1.0f },
    XMVECTOR{ 0.0f, -100.0f, 100.0f }),
  engine_(this, &editor_camera_)
{
  SetVisible(true);
  timer_.Start();

  // Add Models:

  /*Model m1;
  m1.mesh = CreateGrid(100, 100, 2, 2);
  m1.world = XMMatrixIdentity();
  m1.material = Material{
    XMVECTOR{ 0.0f, 1, 0, 1.0f },
    XMVECTOR{ 0.0f, 1.0f, 0, 1.0f },
    XMVECTOR{ 0.0f, 1.0f, 0, 1.0f },
    9.0f
  };

  Model m2;
  m2.mesh = CreateBox(10, 10, 10);
  m2.world = XMMatrixTranslation(-15, 5, 0);
  m2.material = Material{
    XMVECTOR{ 1.0f, 0, 0, 1.0f },
    XMVECTOR{ 1.0f, 0, 0, 1.0f },
    XMVECTOR{ 0.0f, 0, 0, 1.0f },
    9.0f
  };

  Model m3;
  m3.mesh = CreateSphere(5, 40, 40);
  m3.world = XMMatrixTranslation(0, 5, 0);
  m3.material = Material{
    XMVECTOR{ 0.0f, 0, 1, 1.0f },
    XMVECTOR{ 0.0f, 0, 1, 1 },
    XMVECTOR{ 0.0f, 0.0f, 0.0f, 1 },
    9.0f
  };*/

  auto m1_material = Material{
    XMVECTOR{ 1, 1, 1, 1 },
    XMVECTOR{ 1, 1, 1, 1 },
    XMVECTOR{ 1, 1, 1, 1 },
    9.0f
  };
  Model m1(
    engine_.GetDevice(),
    CreateBox(10, 10, 10),
    XMMatrixTranslation(-15, 5, 0),
    m1_material,
    u8"Resource/Textures/WoodCrate02.dds"
    );
  
  std::vector<Model> meshs;
  meshs.push_back(m1);

  engine_.SetModels(meshs);

  // Add Lights:

  AmbientLight ambient{
    XMVECTOR{ 0.15f, 0.15f, 0.15f, 1.0f }
  };
  DirectionalLight directional{
    XMVECTOR{ 0.3f, 0.3f, 0.3f, 1.0f },
    XMVECTOR{ 0, -1.0f, 1.0f }
  };
  PointLight point{
    XMVECTOR{ 1.0f, 1.0f, 1.0f, 1.0f },
    XMVECTOR{ 0, 0.0f, 50.0f, 1.0f },
    Attenuation{ 0, 0.1f, 0 },
    250
  };
  SpotLight spot{
    XMVECTOR{ 1.0f, 1.0f, 1.0f, 1.0f },
    XMVECTOR{ 0, 5.0f, -50.0f, 1.0f },
    XMVECTOR{ 0, 0.0f, 1.0f },
    Attenuation{ 0.0f, 0.1f, 0.0f },
    250.0f,
    200.0f
  };
  lights_.ambients.push_back(ambient);
  //lights_.directionals.push_back(directional);
  lights_.points.push_back(point);
  lights_.spots.push_back(spot);

  engine_.SetLights(lights_);
}

void Editor::Update() {
  if (lights_.spots.size() > 0) {
    lights_.spots[0].position = editor_camera_.GetPosition();
    lights_.spots[0].direction = editor_camera_.GetForwardVector();
  }
  engine_.SetLights(lights_);
  engine_.Update(std::chrono::milliseconds{ 0 }); // TODO: remove fake value.
}

void Editor::Render() {
  engine_.Render();
}

void Editor::OnMouseDown(const MouseButtonEvent & event) {
  last_mouse_position_ = event.position;
}

void Editor::OnMouseUp(const MouseButtonEvent & event) {
}

void Editor::OnMouseMove(const MouseButtonEvent & event) {

  if ((event.button & MouseButton::Left) != 0) {

    // Make each pixel correspond to a quarter of a degree.

    float radian_x = XMConvertToRadians(0.1f*static_cast<float>(event.position.x - last_mouse_position_.x));
    float radian_y = XMConvertToRadians(0.2f*static_cast<float>(event.position.y - last_mouse_position_.y));

    // Update camera

    editor_camera_.Yaw(radian_x);
    editor_camera_.Pitch(radian_y);
  }
  else if ((event.button & MouseButton::Right) != 0) {
    // Get diff to last mouse position
    float d_x = 0.05f * (last_mouse_position_.x - event.position.x);
    float d_y = 0.05f * (event.position.y - last_mouse_position_.y);

    editor_camera_.MoveLeftRight(d_x);
    editor_camera_.MoveUpDown(d_y);
  }

  last_mouse_position_ = event.position;
}

void Editor::OnMouseScroll(const MouseScrollEvent & event) {
  editor_camera_.MoveBackForeward(0.05f * event.distance);
}

void Editor::OnResize() {
  // Change camera aspect ratio.
  auto frustum = editor_camera_.GetFrustum();
  frustum.SetAspectRatio(
    static_cast<float>(GetClientRect().GetSize().w) / GetClientRect().GetSize().h);
  editor_camera_.SetFrustum(frustum);

  engine_.OnResize();
  engine_.Render();  // TODO: condider delete this line, only draw in main loop.
}