#pragma once

#include <assert.h>
#include <string>
#include <plog\Log.h>
#include <DXGI.h>

namespace LL3D {

std::string GetLastErrorAsString();

inline std::wstring GetMessageForHRESULT(HRESULT hr);

// Replacement for std::assert
#ifdef _DEBUG
#define ASSERT(expression) {\
  auto e = expression;\
  if (!e) {\
    LOGF << "Assert failed with "#expression << " = " << e << "\n"\
      << "\tWin32 Error Status:\n"\
      << "\t\tError Code:    " << GetLastError() << "\n"\
      << "\t\tError Message: " << GetLastErrorAsString() << "\n";\
  }\
  assert(e);\
}
#else
#define ASSERT(expression) {\
  auto e = expression;\
  if (!e) {\
    LOGF << "Assert failed with "#expression << " = " << e << "\n"\
      << "\tWin32 Error Status:\n"\
      << "\t\tError Code:    " << GetLastError() << "\n"\
      << "\t\tError Message: " << GetLastErrorAsString() << "\n";\
  }\
}
#endif

// Handle HRESULT error code.
void HR(HRESULT result);

// ASSERT for Win32 error.
#define WA(expression) ASSERT(expression);

}