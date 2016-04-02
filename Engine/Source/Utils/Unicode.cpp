#include "Unicode.h"
#include <Windows.h>

namespace LL3D {

namespace Utils {

std::string Convert(const std::wstring & text, unsigned int code_page /*= CP_UTF8*/) {
  // Create result buffer.
  std::string result;
  result.resize(WideCharToMultiByte(code_page, 0, text.c_str(), static_cast<int>(text.size()),
    nullptr, 0, nullptr, nullptr));

  // Convert
  int s = WideCharToMultiByte(code_page, 0, text.c_str(), static_cast<int>(text.size()),
    const_cast<char*>(result.data()), static_cast<int>(result.size()), nullptr, nullptr);

  return result;
}

std::wstring Convert(const std::string & text, unsigned int code_page /*= CP_UTF8*/) {
  // Create result buffer.
  std::wstring result;
  result.resize(MultiByteToWideChar(code_page, 0, text.c_str(), static_cast<int>(text.size()),
    nullptr, 0));

  // Convert
  int s = MultiByteToWideChar(code_page, 0, text.c_str(), static_cast<int>(text.size()),
    const_cast<wchar_t*>(result.data()), static_cast<int>(result.size()));

  return result;
}

}  // namespace Utils

}

