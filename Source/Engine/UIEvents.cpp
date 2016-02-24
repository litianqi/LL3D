#include "UIEvents.h"

LL3D::MouseScrollEvent::MouseScrollEvent(MouseButton button, IntPoint2 position, int16_t d) :
MouseButtonEvent{button, position},
distance{d}
{
}
