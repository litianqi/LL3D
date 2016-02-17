#pragma once

#include <assert.h>
#include <string>
#include <plog\Log.h>
#include <DXGI.h>

namespace LL3D {

std::string GetLastErrorAsString();

inline std::wstring GetMessageForHRESULT(HRESULT hr);

template<typename T>
void Assert(T expression);
 
// Handle HRESULT error code.
void HR(HRESULT result);

// Handle Win32 error code.
template<typename T>
void WR(T r);

template<typename T>
void Assert(T expression) {
  if (!expression) {
    LOGF << "Assert failed with value = " << expression << "\n"
      << "\tWin32 Error Status:\n"
      << "\t\tError Code:    " << GetLastError() << "\n"
      << "\t\tError Message: " << GetLastErrorAsString() << "\n";
  }

  assert(expression);
}

template<typename T>
void WR(T r) {
  Assert(r);
}

}