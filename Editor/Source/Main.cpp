#include <Windows.h>
#include <plog\Log.h>
#include "Editor.h"
#include "Model.h"
#include "Color.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
  PSTR cmdLine, int showCmd) {
  // Initialize the logger.
  plog::init(plog::debug, "log.txt");

  // Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
  _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

  Editor app;
  app.Run();

  return 0;
}
