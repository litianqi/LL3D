#pragma once

#include <string>
#include <map>
#include <Windows.h>
#include "KeyCode.h"

namespace LL3D {

namespace Input {

class Keyboard {
public:
  
  enum Axis { Horizontal, Vertical };

  
  //--------------------------------------
  // Queries

  //>
  // Returns the keyboard input entered this frame. (Read Only)
  //
  static const std::string& GetInputString();
  //>
  // Returns the value of the virtual axis identified by axis.
  //
  static float GetAxis(Axis axis);
  //>
  // Is any key or mouse button currently held down? (Read Only)
  //
  static bool IsAnyHeldingDown();
  //>
  // Returns true the first frame the user hits any key or mouse button. 
  // (Read Only)
  //
  static bool IsAnyPressed();
  //>
  // Returns true while the user holds down the key identified by the key 
  // KeyCode enum parameter.
  //
  static bool IsHeldingDown(KeyCode key);
  //>
  // Returns true during the frame the user starts pressing down the key 
  // identified by the key KeyCode enum parameter.
  //
  static bool IsPressed(KeyCode key);
  //>
  // Returns true during the frame the user releases the key identified by the 
  // key KeyCode enum parameter.
  //
  static bool IsReleased(KeyCode key);
  
  //>
  // Processes keyboard messages from system. Called by higher message process
  // function.
  //
  static void ProcessMessage(UINT message, WPARAM wparam, LPARAM lparam);
  //>
  // Notifies a new frame has started. Prepares for recording mouse messages in
  // new frame. Called by main loop, like any other Updates.
  //
  static void Update();

private:
  //>
  // Distance per move key(WASD,TDLR) results.
  //
  static const float AxisPerKey;
  //>
  // When helding down a key, it equals press a key PressesPerHeldingDown 
  // times per frame.
  //
  static const float PressesPerHeldingDown;

  static std::string              s_input;
  static std::map<Axis, float>    s_axis;
  static std::map<KeyCode, bool>  s_helding_down;
  static std::map<KeyCode, bool>  s_pressed;
  static std::map<KeyCode, bool>  s_released;
};

}  // namespace Input
}  // namespace LL3D