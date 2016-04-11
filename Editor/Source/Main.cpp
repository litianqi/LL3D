#include <Windows.h>
#include <plog\Log.h>
#include <Scene.h>
#include <Window.h>
#include <Component.h>
#include <Graphics\Device.h>
#include <Core\Exceptions.h>
#include "Editor.h"

using namespace LL3D;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
  PSTR cmdLine, int showCmd) {

  // Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
  _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

  try {
    Editor app;
    app.run();
  }
  catch (ComException& e) {
    LOGF << e.what();
  }
  

  return 0;
}
