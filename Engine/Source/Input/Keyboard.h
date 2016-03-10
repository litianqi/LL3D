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
  //
  // Summary:
  //     ///
  //     Make Game friend for calling Update() in Game.
  //     ///
  friend Game;
  //
  // Summary:
  //     ///
  //     Make Window friend for calling ProcessMessage() in Window.
  //     ///
  friend Window;

public:
  enum Axis { Horizontal, Vertical };

  //
  // Summary:
  //     ///
  //     Returns the keyboard input entered this frame. (Read Only)
  //     ///
  static const std::string& GetInputString();
  //
  // Summary:
  //     ///
  //     Returns the value of the virtual axis identified by axisName.
  //     ///
  //
  // Parameters:
  //   axis:
  static float GetAxis(Axis axis);
  //
  // Summary:
  //     ///
  //     Is any key or mouse button currently held down? (Read Only)
  //     ///
  static bool IsAnyHeldingDown();
  //
  // Summary:
  //     ///
  //     Returns true the first frame the user hits any key or mouse button. (Read Only)
  //     ///
  static bool IsAnyPressed();
  //
  // Summary:
  //     ///
  //     Returns true while the user holds down the key identified by the key KeyCode
  //     enum parameter.
  //     ///
  //
  // Parameters:
  //   key:
  static bool IsHeldingDown(KeyCode key);
  //
  // Summary:
  //     ///
  //     Returns true during the frame the user starts pressing down the key identified
  //     by the key KeyCode enum parameter.
  //     ///
  //
  // Parameters:
  //   key:
  static bool IsPressed(KeyCode key);
  //
  // Summary:
  //     ///
  //     Returns true during the frame the user releases the key identified by the key
  //     KeyCode enum parameter.
  //     ///
  //
  // Parameters:
  //   key:
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