#pragma once

#include <assert.h>
#include <string>
#include <plog\Log.h>
#include <DXGI.h>

namespace LL3D
{

  // Helper for output debug tracing
  inline void DebugTrace(_In_z_ _Printf_format_string_ const char* format, ...)
  {
  #ifdef _DEBUG
    va_list args;
    va_start(args, format);

    char buff[1024] = { 0 };
    vsprintf_s(buff, format, args);
    OutputDebugStringA(buff);
    va_end(args);
  #else
    UNREFERENCED_PARAMETER(format);
  #endif
  }

  std::string lastErrorAsString();

  // Replacement for std::assert
#ifdef _DEBUG
#define ASSERT(expression) {\
  auto e = expression;\
  if (!e) {\
    LOGF << "Assert failed with "#expression << " = " << e << "\n"\
      << "\tWin32 Error Status:\n"\
      << "\t\tError Code:    " << GetLastError() << "\n"\
      << "\t\tError Message: " << lastErrorAsString() << "\n";\
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
      << "\t\tError Message: " << lastErrorAsString() << "\n";\
  }\
}
#endif

// ASSERT for Win32 error.
#define WA(expression) ASSERT(expression);

}