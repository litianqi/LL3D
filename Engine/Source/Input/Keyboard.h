#pragma once

#include <string>
#include <map>
#include <Windows.h>
#include "Input/KeyCode.h"

namespace LL3D
{
  namespace Input
  {

    class Keyboard
    {
    public:

      enum Axis { Horizontal, Vertical };

      // Returns the keyboard input entered this frame. (Read Only)
      static const std::string& inputString();

      // Returns the value of the virtual axis identified by axis.
      static float axis(Axis axis);

      // Is any key or mouse button currently held down? (Read Only)
      static bool isAnyHeldingDown();

      // Returns true the first frame the user hits any key or mouse button. 
      // (Read Only)
      static bool isAnyPressed();

      // Returns true while the user holds down the key identified by the key 
      // KeyCode enum parameter.
      static bool isHeldingDown(KeyCode key);

      // Returns true during the frame the user starts pressing down the key 
      // identified by the key KeyCode enum parameter.
      static bool isPressed(KeyCode key);

      // Returns true during the frame the user releases the key identified by the 
      // key KeyCode enum parameter.
      static bool isReleased(KeyCode key);


      // Processes keyboard messages from system. Called by higher message process
      // function.
      static void processMessage(UINT message, WPARAM wparam, LPARAM lparam);

      // Notifies a new frame has started. Prepares for recording mouse messages in
      // new frame. Called by main loop, like any other Updates.
      static void update();

    private:

      // Distance per move key(WASD,TDLR) results.
      static const float kAxisPerKey;

      // When helding down a key, it equals press a key PressesPerHeldingDown 
      // times per frame.
      static const float kPressesPerHeldingDown;

      static std::string              s_input;
      static std::map<Axis, float>    s_axis;
      static std::map<KeyCode, bool>  s_heldingDown;
      static std::map<KeyCode, bool>  s_pressed;
      static std::map<KeyCode, bool>  s_released;

    };

  }  // namespace Input
}  // namespace LL3D