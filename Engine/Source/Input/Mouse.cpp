#include "Mouse.h"
#include <windowsx.h>
#include "..\Core\Assert.h"

namespace LL3D {
namespace Input {

void Mouse::ProcessMessage(UINT message, WPARAM wparam, LPARAM lparam) {
  switch (message) {
    case WM_MOUSELEAVE:
    {
      for (int btn = Left; btn < Count; btn++) {
        s_released[static_cast<Button>(btn)] = true;
        s_helding_down[static_cast<Button>(btn)] = false;
      }
      return;
    }
    case WM_MOUSEHOVER:
    {
      break;
    }
    case WM_LBUTTONDOWN:
    {
      s_pressed[Left] = true;
      s_helding_down[Left] = true;
      break;
    }
    case WM_MBUTTONDOWN:
    {
      s_pressed[Middle] = true;
      s_helding_down[Middle] = true;
      break;
    }
    case WM_RBUTTONDOWN:
    {
      s_pressed[Right] = true;
      s_helding_down[Right] = true;
      break;;
    }
    case WM_LBUTTONUP:
    {
      s_released[Left] = true;
      s_helding_down[Left] = false;
      break;
    }
    case WM_MBUTTONUP:
    {
      s_released[Middle] = true;
      s_helding_down[Middle] = false;
      break;
    }
    case WM_RBUTTONUP:
    {
      s_released[Right] = true;
      s_helding_down[Right] = false;
      break;
    }
    case WM_MOUSEMOVE:
    {
      // TODO
      break;
    }
    case WM_MOUSEWHEEL:
    {
      s_scroll_delta += GET_WHEEL_DELTA_WPARAM(wparam);
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

void Mouse::Update() {
  s_scroll_delta = 0;
  s_pressed.clear();
  s_released.clear();
}

void Mouse::Init(HWND hwnd)
{
  s_hwnd = hwnd;
}

Math::Vector2 Mouse::GetPosition() {
  return s_position;
}

int Mouse::GetScrollDelta() {
  return s_scroll_delta;
}

bool Mouse::IsHeldingDown(Button button) {
  return s_helding_down[button];
}

bool Mouse::IsPressed(Button button) {
  return s_pressed[button];
}

bool Mouse::IsReleased(Button button) {
  return s_released[button];
}

Math::Vector2                 Mouse::s_position;
int                           Mouse::s_scroll_delta;
std::map<Mouse::Button, bool> Mouse::s_helding_down;
std::map<Mouse::Button, bool> Mouse::s_pressed;
std::map<Mouse::Button, bool> Mouse::s_released;
HWND                          Mouse::s_hwnd;

}  // namespace Input
}  // namespace LL3D