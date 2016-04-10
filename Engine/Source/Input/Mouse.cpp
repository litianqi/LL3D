#include "Input/Mouse.h"
#include <windowsx.h>
#include "Core/Assert.h"

namespace LL3D {
namespace Input {

void Mouse::processMessage(UINT message, WPARAM wparam, LPARAM lparam) {
  switch (message) {
    case WM_MOUSELEAVE:
    {
      for (int btn = kLeft; btn < kCount; btn++) {
        s_released[static_cast<Button>(btn)] = true;
        s_heldingDown[static_cast<Button>(btn)] = false;
      }
      return;
    }
    case WM_MOUSEHOVER:
    {
      break;
    }
    case WM_LBUTTONDOWN:
    {
      s_pressed[kLeft] = true;
      s_heldingDown[kLeft] = true;
      break;
    }
    case WM_MBUTTONDOWN:
    {
      s_pressed[kMiddle] = true;
      s_heldingDown[kMiddle] = true;
      break;
    }
    case WM_RBUTTONDOWN:
    {
      s_pressed[kRight] = true;
      s_heldingDown[kRight] = true;
      break;;
    }
    case WM_LBUTTONUP:
    {
      s_released[kLeft] = true;
      s_heldingDown[kLeft] = false;
      break;
    }
    case WM_MBUTTONUP:
    {
      s_released[kMiddle] = true;
      s_heldingDown[kMiddle] = false;
      break;
    }
    case WM_RBUTTONUP:
    {
      s_released[kRight] = true;
      s_heldingDown[kRight] = false;
      break;
    }
    case WM_MOUSEMOVE:
    {
      // TODO
      break;
    }
    case WM_MOUSEWHEEL:
    {
      s_scrollDelta += GET_WHEEL_DELTA_WPARAM(wparam);
      break;
    }
    default:
    {
      // Not a mouse message, so exit
      return;
    }
  }

  auto track = TRACKMOUSEEVENT{
    sizeof(TRACKMOUSEEVENT),
    TME_HOVER | TME_LEAVE,
    s_hwnd,
    HOVER_DEFAULT
  };
  ASSERT(TrackMouseEvent(&track));

  // All mouse messages provide a new pointer position
  s_position.x = static_cast<short>(LOWORD(lparam)); // GET_X_LPARAM(lParam);
  s_position.y = static_cast<short>(HIWORD(lparam)); // GET_Y_LPARAM(lParam);
}

void Mouse::update() {
  s_scrollDelta = 0;
  s_pressed.clear();
  s_released.clear();
}

void Mouse::initialize(HWND hwnd)
{
  s_hwnd = hwnd;
}

Math::Vector2 Mouse::position() {
  return s_position;
}

int Mouse::scrollDelta() {
  return s_scrollDelta;
}

bool Mouse::isHeldingDown(Button button) {
  return s_heldingDown[button];
}

bool Mouse::isPressed(Button button) {
  return s_pressed[button];
}

bool Mouse::isReleased(Button button) {
  return s_released[button];
}

Math::Vector2                 Mouse::s_position;
int                           Mouse::s_scrollDelta;
std::map<Mouse::Button, bool> Mouse::s_heldingDown;
std::map<Mouse::Button, bool> Mouse::s_pressed;
std::map<Mouse::Button, bool> Mouse::s_released;
HWND                          Mouse::s_hwnd;

}  // namespace Input
}  // namespace LL3D