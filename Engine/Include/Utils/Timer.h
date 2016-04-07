#pragma once

#include <cstdint>

namespace LL3D {
namespace Utils {

class Stopwatch {
public:
  Stopwatch();

  void Start();
  void Pause();
  void Resume();

  int RunDurationMs();
  float RunDurationS();

private:
  enum State { Unstarted, Running, Paused };

  State current_state_ = Unstarted;

  uint64_t start_time_;
  uint64_t pause_time_;
  uint64_t paused_duration_;

  static float seconds_per_count_;
};

}  // namespace Utils
}  // namespace LL3D


