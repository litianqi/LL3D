#pragma once

#include <cstdint>

namespace LL3D
{
  namespace Utils
  {
    class Stopwatch
    {
    public:
      Stopwatch();

      void start();
      void pause();
      void resume();

      int durationMs();
      float durationS();

    private:
      enum State { kUnstarted, kRunning, kPaused };

      State curState = kUnstarted;
      uint64_t startPoint_;
      uint64_t pausePoint_;
      uint64_t pauseDuration_;

      static float secondsPerCount_;
    };

  }  // namespace Utils
}  // namespace LL3D


