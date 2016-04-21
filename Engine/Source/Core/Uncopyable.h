#pragma once

namespace LL3D {
namespace Core {

class Uncopyable
{
public:
  Uncopyable(const Uncopyable&) = delete;
  Uncopyable& operator=(const Uncopyable&) = delete;

protected:
  Uncopyable() = default;
  ~Uncopyable() = default;
};

} // namespace Core
} // namespace LL3D