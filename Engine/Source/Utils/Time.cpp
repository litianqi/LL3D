#include "Utils/Time.h"

using namespace std::chrono;
using namespace LL3D;

namespace {

bool s_firstFrame = true;
system_clock::time_point s_lastFrameStartPoint;
system_clock::time_point s_frameStartPoint;
unsigned s_frameCount = 0u;

} // namespace

namespace LL3D {
namespace Utils {
namespace Time {

void beginFrame()
{
  if (s_firstFrame) {
    s_frameStartPoint = system_clock::now();
    s_firstFrame = false;
  }

  s_frameCount++;
  s_lastFrameStartPoint = s_frameStartPoint;
  s_frameStartPoint = system_clock::now();
}

Time::seconds deltaTime()
{
  return std::chrono::duration_cast<Time::seconds>(s_frameStartPoint -
                                                   s_lastFrameStartPoint);
}

unsigned frameCount()
{
  return s_frameCount;
}

} // namespace Time
} // namespace Utils
} // namespace LL3D
