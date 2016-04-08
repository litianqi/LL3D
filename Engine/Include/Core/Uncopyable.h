#pragma once

namespace LL3D {

class Uncopyable {
public:
  Uncopyable(const Uncopyable&) = delete;
  Uncopyable& operator=(const Uncopyable&) = delete;

protected:
  Uncopyable() = default;
  ~Uncopyable() = default;
};

}  // namespace LL3D