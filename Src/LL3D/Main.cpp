#include <Windows.h>
#include <plog\Log.h>
#include "Application.h"
#include "Mesh.h"
#include "Color.h"

using namespace LL3D;
using namespace DirectX;

class TestApplication : public Application {
public:
  TestApplication();
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
  PSTR cmdLine, int showCmd) {
  // Initialize the logger.
  plog::init(plog::debug, "log.txt");

  // Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
  _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

  TestApplication app;
  app.SetMaxFPS(60);
  app.Run();

  return 0;
}

TestApplication::TestApplication() {
  auto mesh_grid = CreateGrid(100, 100, 5, 5, Colors::Silver);
  
  auto tranlate_box = XMMatrixTranslation(0, 5, 0);
  auto mesh_box = CreateBox(10, 10, 10, Colors::Blue, tranlate_box);
  
  auto tranlate_sphere = XMMatrixTranslation(15, 5, 0);
  auto mesh_sphere = CreateSphere(5, 40, 40, Colors::Green, tranlate_sphere);
  
  std::vector<MeshData> meshs;
  meshs.push_back(mesh_grid);
  meshs.push_back(mesh_box);
  meshs.push_back(mesh_sphere);

  engine_.SetMesh(meshs);
}
