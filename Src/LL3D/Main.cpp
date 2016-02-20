#include <Windows.h>
#include <plog\Log.h>
#include "Application.h"
#include "Model.h"
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
  Model m1;
  m1.mesh = CreateGrid(100, 100, 2, 2);
  m1.world = XMMatrixIdentity();
  m1.material = Material{
    XMVECTOR{ 1.0f, 0, 0, 1.0f },
    XMVECTOR{ 0.0f, 1.0f, 0, 1.0f },
    XMVECTOR{ 0.0f, 1.0f, 0, 1.0f },
    9.0f
  };

  Model m2;
  m2.mesh = CreateBox(10, 10, 10);
  m2.world = XMMatrixTranslation(-15, 5, 0);
  m2.material = Material{
    XMVECTOR{ 0.0f, 0, 0, 1.0f },
    XMVECTOR{ 0.0f, 0, 0, 1.0f },
    XMVECTOR{ 1.0f, 0, 0, 1.0f },
    9.0f
  };

  Model m3;
  m3.mesh = CreateSphere(5, 40, 40);
  m3.world = XMMatrixTranslation(0, 5, 0);
  m3.material = Material{
    XMVECTOR{ 1.0f, 0, 0 },
    XMVECTOR{ 0.0f, 0, 1 },
    XMVECTOR{ 0.0f, 0.0f, 1.0f },
    9.0f
  };

  std::vector<Model> meshs;
  meshs.push_back(m1);
  meshs.push_back(m2);
  meshs.push_back(m3);

  engine_.SetModels(meshs);

  DirectionalLight directional{
    XMVECTOR{1.0f, 1.0f, 1.0f, 1.0f},
    XMVECTOR{ 0, -1.0f, 1.0f}
  };
  Lights lights;
  lights.directionals.push_back(directional);

  engine_.SetLights(lights);
}
