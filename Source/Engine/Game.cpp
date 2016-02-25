#include "Game.h"

void LL3D::Game::Run() {
  MSG msg = { 0 };

  while (msg.message != WM_QUIT) {
    // If there are Window messages then process them.
    if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }

    Sleep(20);  // TODO: remove hardcode.
    Update();
    Render();
  }
}
