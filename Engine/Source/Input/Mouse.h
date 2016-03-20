#pragma once

#include <Windows.h>
#include <map>
#include "../Math/Math.h"

namespace LL3D {

namespace Input {

class Mouse {
public:

  
  //--------------------------------------
  // Types

  enum Button {Left, Middle, Right, X1, X2};
  

  //--------------------------------------
  // Queries

  //>
  // The current mouse position in pixel coordinates.
  //
  static Math::Vector2 GetPosition();
  //>
  // The current mouse scroll delta.
  //
  static int GetScrollDelta();
  //>
  // Returns true while the virtual button identified by button is held down.
  //
  static bool IsHeldingDown(Button button);
  //>
  // Returns true during the frame the user pressed down the virtual button
  // identified by button.
  //
  static bool IsPressed(Button button);
  //>
  // Returns true the first frame the user releases the virtual button 
  // identified by button.
  //
  static bool IsReleased(Button button);

  
  //--------------------------------------
  // Operations

  //>
  // Processes mouse messages from system. Called by higher message process
  // function.
  //
  static void ProcessMessage(UINT message, WPARAM wparam, LPARAM lparam);
  //>
  // Notifies a new frame started. Prepares for recording mouse messages in
  // new frame. Called by main loop, like any other Updates.
  //
  static void Update();

private:
  static Math::Vector2           s_position;
  static int                     s_scroll_delta;
  static std::map<Button, bool>  s_helding_down;
  static std::map<Button, bool>  s_pressed;
  static std::map<Button, bool>  s_released;
};

}  // namespace Input
}  // namespace LL3D
