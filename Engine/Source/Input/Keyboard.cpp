#include "Input/Keyboard.h"
#include <codecvt>

using namespace std;

namespace LL3D
{
  namespace Input
  {

    const std::string& Keyboard::inputString()
    {
      return s_input;
    }

    float Keyboard::axis(Axis axis)
    {
      //>
      // When a key is holding down, its possible it receives none OS message.
      // We mock a message result anyway.
      //
      if (axis == Vertical && s_axis[Vertical] == 0)
      {
        if (isHeldingDown(W) || isHeldingDown(Up))
          s_axis[Vertical] += kPressesPerHeldingDown * kAxisPerKey;
        else if (isHeldingDown(S) || isHeldingDown(Down))
          s_axis[Vertical] -= kPressesPerHeldingDown * kAxisPerKey;
      }
      if (axis == Horizontal && s_axis[Horizontal] == 0)
      {
        if (isHeldingDown(D) || isHeldingDown(Right))
          s_axis[Horizontal] += kPressesPerHeldingDown * kAxisPerKey;
        else if (isHeldingDown(A) || isHeldingDown(Left))
          s_axis[Horizontal] -= kPressesPerHeldingDown * kAxisPerKey;
      }

      return s_axis[axis];
    }

    bool Keyboard::isAnyHeldingDown()
    {
      for (auto key : s_heldingDown)
      {
        if (key.second)
          return true;
      }
      return false;
    }

    bool Keyboard::isAnyPressed()
    {
      for (auto key : s_pressed)
      {
        if (key.second)
          return true;
      }
      return false;
    }

    bool Keyboard::isHeldingDown(KeyCode key)
    {
      return s_heldingDown[key];
    }

    bool Keyboard::isPressed(KeyCode key)
    {
      return s_pressed[key];
    }

    bool Keyboard::isReleased(KeyCode key)
    {
      return s_released[key];
    }

    const float Keyboard::kAxisPerKey = 0.5f;
    const float Keyboard::kPressesPerHeldingDown = 0.5f;

    void Keyboard::processMessage(UINT message, WPARAM wparam, LPARAM lparam)
    {
      bool down = false;

      switch (message)
      {
        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
        case WM_KEYUP:
        case WM_SYSKEYUP:
        {
          switch (message)
          {
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
          switch (vk)
          {
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
              s_axis[Vertical] += kAxisPerKey;
              break;

            case Down:
            case S:
              s_axis[Vertical] -= kAxisPerKey;
              break;

            case Right:
            case D:
              s_axis[Horizontal] += kAxisPerKey;
              break;

            case Left:
            case A:
              s_axis[Horizontal] -= kAxisPerKey;
              break;
          }

          if (down)
          {
            s_pressed[static_cast<KeyCode>(vk)] = true;
            s_heldingDown[static_cast<KeyCode>(vk)] = true;
          }
          else
          {
            s_released[static_cast<KeyCode>(vk)] = true;
            s_heldingDown[static_cast<KeyCode>(vk)] = false;
          }
          break;
        }
        case WM_CHAR:
        {
          switch (wparam)
          {
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

    void Keyboard::update()
    {
      s_axis.clear();
      s_input.clear();
      s_pressed.clear();
      s_released.clear();
    }

    string                      Keyboard::s_input;
    map<Keyboard::Axis, float>  Keyboard::s_axis;
    map<KeyCode, bool>          Keyboard::s_heldingDown;
    map<KeyCode, bool>          Keyboard::s_pressed;
    map<KeyCode, bool>          Keyboard::s_released;

  }  // namespace Input
}  // namespace LL3D