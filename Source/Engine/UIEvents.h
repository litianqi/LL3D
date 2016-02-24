#pragma once

#include <Windows.h>
#include "Type.h"

namespace LL3D {

enum MouseButton {
  Control = MK_CONTROL,
  Left = MK_LBUTTON,
  Middle = MK_MBUTTON,
  Right = MK_RBUTTON,
  Shift = MK_SHIFT,
  X1 = MK_XBUTTON1,
  X2 = MK_XBUTTON2,
};

struct MouseButtonEvent {
  MouseButton button;  // Could be a combiniation of a few buttons.
  IntPoint2 position;
};

struct MouseScrollEvent: public MouseButtonEvent {
  MouseScrollEvent(MouseButton button, IntPoint2 position, int16_t d);
  int16_t distance;
};

}  // namespace LL3D