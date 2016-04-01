#include "Timer.h"
#include <Windows.h>

namespace LL3D {
namespace Utils {

float Stopwatch::seconds_per_count_;

Stopwatch::Stopwatch() {
  uint64_t counts_per_second;
  QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&counts_per_second));
  seconds_per_count_ = 1.0f / counts_per_second;
}

void Stopwatch::Start() {
  current_state_ = Running;
  paused_duration_ = 0;
  QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&start_time_));
}

void Stopwatch::Pause() {
  if (current_state_ == Running) {
    current_state_ = Paused;
    QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&pause_time_));
  }
}

void Stopwatch::Resume() {
  if (current_state_ == Paused) {
    current_state_ = Running;
    uint64_t resume_time;
    QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&resume_time));
    paused_duration_ += resume_time - pause_time_;
  }
}

int Stopwatch::RunDurationMs() {
  return static_cast<int>(RunDurationS() * 1000.0);
}

float Stopwatch::RunDurationS() {
  if (current_state_ == Unstarted) {
    return 0.0;
  }
  else if (current_state_ == Running) {
    uint64_t current_time;
    QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&current_time));
    return (current_time - start_time_ - paused_duration_) * seconds_per_count_;
  }
  else {  // current_state_ == Paused
    return (pause_time_ - start_time_ - paused_duration_) * seconds_per_count_;
  }
}

}  // namespace Utils
}  // namespace LL3D

