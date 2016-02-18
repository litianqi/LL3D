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
  auto mesh_grid = CreateGrid(100, 100, 5, 5, XMVECTOR{0.8f, 0.5f, 1.0f, 1.0f});
  auto mesh_box = CreateBox(10, 10, 10, Colors::Cyan);
  engine_.SetMesh(mesh_grid + mesh_box);
}
