#include "Unicode.h"
#include <Windows.h>
#include "..\Core\Assert.h"

namespace LL3D {
namespace Utils {

std::string Convert(const std::wstring & text, unsigned int code_page /*= CP_UTF8*/) {
  // Create result with proper size.
  std::string result;
  result.resize(WideCharToMultiByte(code_page, 0, text.c_str(), static_cast<int>(text.size()),
    nullptr, 0, nullptr, nullptr));

  // Do convert
  int size = WideCharToMultiByte(code_page, 0, text.c_str(), static_cast<int>(text.size()),
    const_cast<char*>(result.data()), static_cast<int>(result.size()), nullptr, nullptr);
  ASSERT(size == result.size());

  return result;
}

std::wstring Convert(const std::string & text, unsigned int code_page /*= CP_UTF8*/) {
  // Create result with proper size.
  std::wstring result;
  result.resize(MultiByteToWideChar(code_page, 0, text.c_str(), static_cast<int>(text.size()),
    nullptr, 0));

  // Do convert
  int size = MultiByteToWideChar(code_page, 0, text.c_str(), static_cast<int>(text.size()),
    const_cast<wchar_t*>(result.data()), static_cast<int>(result.size()));
  ASSERT(size == result.size());

  return result;
}

}  // namespace Utils
}  // namespace LL3D 

