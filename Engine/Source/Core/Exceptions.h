#pragma once

#include <exception>
#include <stdexcept>
#include <DXGI.h>

namespace LL3D {

// Helper class for COM exceptions
class ComException : public std::exception {
public:
  ComException(HRESULT hr);

  const char* what() const override;

private:
  std::string what_;
};

// Helper utility converts D3D API failures into exceptions.
inline void throwIfFailed(HRESULT hr) {
  if (FAILED(hr)) {
    throw ComException(hr);
  }
}

class NotFound : public std::runtime_error {
public:
  NotFound(const std::string& what);
};

class InvalidArgument : public std::runtime_error {
public:
  InvalidArgument(const std::string& what);
};

}  // namespace LL3D
