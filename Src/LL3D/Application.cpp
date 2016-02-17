#include "Application.h"
#include <plog\Log.h>

namespace LL3D {

Application::Application():
engine_(this)
{
  timer_.Start();
}

void Application::Run() {
  MSG msg = { 0 };

  while (msg.message != WM_QUIT) {
    // If there are Window messages then process them.
    if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
    // Otherwise, do animation/game stuff.
    else {
      if (max_fps_ > 0) {
        // If time spent sine last frame less than max_fps_, sleep
        int td_spent = timer_.RunDurationMs() - td_before_last_draw_;
        const int td_per_frame = static_cast<int>(1000.0 / max_fps_);
        if (td_spent <td_per_frame) {
          Sleep(td_per_frame - td_spent);
        }
      }
      td_before_last_draw_ = timer_.RunDurationMs();
      // TODO: engine_.Update(0);
      engine_.Draw();
    }
  }

  // TODO: Is everything ok without "return (int)msg.wParam;"?
}

void Application::SetMaxFPS(int fps) {
  max_fps_ = fps;
}

void Application::OnMouseDown(const MouseButtonEvent & event) {
  engine_.OnMouseDown(event);
}

void Application::OnMouseUp(const MouseButtonEvent & event) {
  engine_.OnMouseUp(event);
}

void Application::OnMouseMove(const MouseButtonEvent & event) {
  engine_.OnMouseMove(event);
}

void Application::OnMouseScroll(const MouseScrollEvent & event) {
  engine_.OnMouseScroll(event);
}

void Application::OnResize() {
  engine_.OnResize();
}

}  // namespace LL3D 