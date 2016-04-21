#include "Utils/Timer.h"
#include <Windows.h>

namespace LL3D {
namespace Utils {
float Stopwatch::secondsPerCount_;

Stopwatch::Stopwatch()
{
  uint64_t counts_per_second;
  QueryPerformanceFrequency(
    reinterpret_cast<LARGE_INTEGER*>(&counts_per_second));
  secondsPerCount_ = 1.0f / counts_per_second;
}

void
Stopwatch::start()
{
  curState = kRunning;
  pauseDuration_ = 0;
  QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&startPoint_));
}

void
Stopwatch::pause()
{
  if (curState == kRunning) {
    curState = kPaused;
    QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&pausePoint_));
  }
}

void
Stopwatch::resume()
{
  if (curState == kPaused) {
    curState = kRunning;
    uint64_t resume_time;
    QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&resume_time));
    pauseDuration_ += resume_time - pausePoint_;
  }
}

int
Stopwatch::durationMs()
{
  return static_cast<int>(durationS() * 1000.0);
}

float
Stopwatch::durationS()
{
  if (curState == kUnstarted) {
    return 0.0;
  } else if (curState == kRunning) {
    uint64_t current_time;
    QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&current_time));
    return (current_time - startPoint_ - pauseDuration_) * secondsPerCount_;
  } else { // current_state_ == Paused
    return (pausePoint_ - startPoint_ - pauseDuration_) * secondsPerCount_;
  }
}

} // namespace Utils
} // namespace LL3D
