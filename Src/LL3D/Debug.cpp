#include "Debug.h"
#include <d3d9.h>
#include <comdef.h>

namespace LL3D {

//Returns the last Win32 error, in string format. Returns an empty string if there is no error.
std::string GetLastErrorAsString() {
  //Get the error message, if any.
  DWORD errorMessageID = ::GetLastError();
  if (errorMessageID == 0)
    return std::string(); //No error message has been recorded

  LPSTR messageBuffer = nullptr;
  size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
    NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

  std::string message(messageBuffer, size);

  //Free the buffer.
  LocalFree(messageBuffer);

  return message;
}

std::wstring GetMessageForHRESULT(HRESULT hr) {
  _com_error err(hr);
  return std::wstring(err.ErrorMessage());
}

void HR(HRESULT hr) {
  if (FAILED(hr)) {  
    LOGF << "HR Failed!\n"
      << "\tHR Error Status:\n"
      << "\t\tError Code:    " << hr << "(0x" << std::hex  << hr << ")" << "\n"
      << "\t\tError Message: " << GetMessageForHRESULT(hr) << "\n"
      << "\tWin32 Error Status:\n"
      << "\t\tError Code:    " << GetLastError() << "\n"
      << "\t\tError Message: " << GetLastErrorAsString() << "\n";
    assert(false);
  }
}

}  // namespace LL3D
