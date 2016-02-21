#include "Application.h"

void LL3D::Application::Run() {
  MSG msg = { 0 };

  while (msg.message != WM_QUIT) {
    // If there are Window messages then process them.
    if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }

    Update();
  }
}
