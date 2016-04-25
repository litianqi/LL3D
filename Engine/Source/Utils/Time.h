#pragma once

#include <chrono>
#include "Utils/Time.h"

namespace LL3D {
namespace Utils {
namespace Time {

using seconds = std::chrono::duration<double>;

/**
 * Notify Time class, a new frame is beginning to start.
 */
void beginFrame();

/**
 * The time in seconds it took to complete the last frame.
 */
seconds deltaTime();

/**
 * The total number of frames that have passed.
 */
unsigned frameCount();

} // namespace Time
} // namespace Utils
} // namespace LL3D
