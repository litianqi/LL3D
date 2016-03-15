#include "Keyboard.h"
#include <codecvt>

using namespace std;

namespace LL3D {
namespace Input {

const std::string& Keyboard::GetInputString() {
  return s_input;
}

float Keyboard::GetAxis(Axis axis) {
  return s_axis[axis];
}

bool Keyboard::IsAnyHeldingDown() {
  for (auto key : s_helding_down) {
    if (key.second)
      return true;
  }
  return false;
}

bool Keyboard::IsAnyPressed() {
  for (auto key : s_pressed) {
    if (key.second)
      return true;
  }
  return false;
}

bool Keyboard::IsHeldingDown(KeyCode key) {
  return s_helding_down[key];
}

bool Keyboard::IsPressed(KeyCode key) {
  return s_pressed[key];
}

bool Keyboard::IsReleased(KeyCode key) {
  return s_released[key];
}

const float Keyboard::AxisPerKey = 1.0f;

void Keyboard::ProcessMessage(UINT message, WPARAM wparam, LPARAM lparam) {
  bool down = false;

  switch (message) {
    case WM_KEYDOWN:
    case WM_SYSKEYDOWN:
    case WM_KEYUP:
    case WM_SYSKEYUP:
    {
      switch (message) {
        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
          down = true;
          break;

        case WM_KEYUP:
        case WM_SYSKEYUP:
          break;
      }

      // Do key-specific stuff.
      int vk = static_cast<int>(wparam);
      switch (vk) {
        case VK_SHIFT:
          vk = MapVirtualKey((lparam & 0x00ff0000) >> 16, MAPVK_VSC_TO_VK_EX);
          break;

        case VK_CONTROL:
          vk = (lparam & 0x01000000) ? VK_RCONTROL : VK_LCONTROL;
          break;

        case VK_MENU:
          vk = (lparam & 0x01000000) ? VK_RMENU : VK_LMENU;
          break;

        case Up:
        case W:
          s_axis[Vertical] += AxisPerKey;
          break;

        case Down:
        case S:
          s_axis[Vertical] -= AxisPerKey;
          break;

        case Right:
        case D:
          s_axis[Horizontal] += AxisPerKey;
          break;

        case Left:
        case A:
          s_axis[Horizontal] -= AxisPerKey;
          break;
      }

      if (down) {
        s_pressed[static_cast<KeyCode>(vk)] = true;
        s_helding_down[static_cast<KeyCode>(vk)] = true;
      }
      else {
        s_released[static_cast<KeyCode>(vk)] = true;
        s_helding_down[static_cast<KeyCode>(vk)] = false;
      }
      break;
    }
    case WM_CHAR:
    {
      switch (wparam) {
        case 0x08:  // backspace 
        {
          if (s_input.size() > 0)
            s_input.erase(s_input.end() - 1);
        }
        case 0x0A:  // linefeed 
        case 0x1B:  // escape 
        case 0x09:  // tab 
        case 0x0D:  // carriage return 
        default:    // displayable character 
          s_input.push_back(static_cast<char>(wparam));
      }
      break;
    }
    default:
      return;
  }
}

void Keyboard::Update() {
  s_input.clear();
  s_axis.clear();
  s_helding_down.clear();
  s_pressed.clear();
  s_released.clear();
}

string                      Keyboard::s_input;
map<Keyboard::Axis, float>  Keyboard::s_axis;
map<KeyCode, bool>          Keyboard::s_helding_down;
map<KeyCode, bool>          Keyboard::s_pressed;
map<KeyCode, bool>          Keyboard::s_released;

}  // namespace Input
}  // namespace LL3D