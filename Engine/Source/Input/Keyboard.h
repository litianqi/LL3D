#pragma once

#include <string>
#include <map>
#include <Windows.h>
#include "KeyCode.h"

namespace LL3D {

class Game;
class Window;

namespace Input {

class Keyboard {
  friend Game;
  friend Window;

public:
  enum Axis { Horizontal, Vertical };

  static std::string GetInputString();
  static float GetAxis(Axis axis);

  static bool IsAnyHeldingDown();
  static bool IsAnyPressed();
  static bool IsHeldingDown(KeyCode key);
  static bool IsPressed(KeyCode key);
  static bool IsReleased(KeyCode key);
  
private:
  static const float AxisPerKey;

  static void ProcessMessage(UINT message, WPARAM wparam, LPARAM lparam);
  static void Update();

  static std::string              s_input;
  static std::map<Axis, float>    s_axis;
  static std::map<KeyCode, bool>  s_helding_down;
  static std::map<KeyCode, bool>  s_pressed;
  static std::map<KeyCode, bool>  s_released;
};

}  // namespace Input
}  // namespace LL3D