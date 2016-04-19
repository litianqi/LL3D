#pragma once

#include <Windows.h>
#include <map>
#include "Math/Math.h"

namespace LL3D
{
  namespace Input
  {
    class Mouse
    {
    public:

      enum Button { kLeft, kMiddle, kRight, kX1, kX2, kCount };

      // The current mouse position in pixel coordinates.
      static Math::Vector2 position();

      // The current mouse scroll delta.
      static int scrollDelta();

      // Returns true while the virtual button identified by button is held down.
      static bool isHeldingDown(Button button);

      // Returns true during the frame the user pressed down the virtual button
      // identified by button.
      static bool isPressed(Button button);

      // Returns true the first frame the user releases the virtual button 
      // identified by button.
      static bool isReleased(Button button);


      static void initialize(HWND hwnd);

      // Processes mouse messages from system. Called by higher message process
      // function.
      static void processMessage(UINT message, WPARAM wparam, LPARAM lparam);

      // Notifies a new frame started. Prepares for recording mouse messages in
      // new frame. Called by main loop, like any other Updates.
      static void update();

    private:

      static Math::Vector2           s_position;
      static int                     s_scrollDelta;
      static std::map<Button, bool>  s_heldingDown;
      static std::map<Button, bool>  s_pressed;
      static std::map<Button, bool>  s_released;
      static HWND                    s_hwnd;

    };
  }  // namespace Input
}  // namespace LL3D
