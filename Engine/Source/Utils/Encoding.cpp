#include "Encoding.h"
#include <vector>
#include <Windows.h>

namespace LL3D {

namespace Utils {

std::string UTF16ToUTF8(const std::wstring & text) {

  // Create result buffer.
  std::string result;
  result.resize(WideCharToMultiByte(CP_UTF8, 0, text.c_str(), static_cast<int>(text.size()),
    nullptr, 0, nullptr, nullptr));

  // Convert
  int s = WideCharToMultiByte(CP_UTF8, 0, text.c_str(), static_cast<int>(text.size()),
    const_cast<char*>(result.data()), static_cast<int>(result.size()), nullptr, nullptr);

  return result;
}

std::wstring UTF8ToUTF16(const std::string & text) {

  // Create result buffer.
  std::wstring result;
  result.resize(MultiByteToWideChar(CP_UTF8, 0, text.c_str(), static_cast<int>(text.size()),
    nullptr, 0));

  // Convert
  int s = MultiByteToWideChar(CP_UTF8, 0, text.c_str(), static_cast<int>(text.size()),
    const_cast<wchar_t*>(result.data()), static_cast<int>(result.size()));

  return result;
}

std::string Convert(const std::wstring & str, unsigned int code_page /*= CP_UTF8*/) {
  if (code_page == CP_UTF8) {
    return UTF16ToUTF8(str);
  }
  else {
    return std::string();  // TODO
  }
}

std::wstring Convert(const std::string & str, unsigned int code_page /*= CP_UTF8*/) {
  if (code_page == CP_UTF8) {
    return UTF8ToUTF16(str);
  }
  else {
    return std::wstring();  // TODO
  }
}

}  // namespace Utils

//std::wstring LocalToUTF16(const std::string & text) {
//  
//  // Create result buffer.
//  std::wstring result;
//  result.resize(MultiByteToWideChar(CP_ACP, 0, text.c_str(), static_cast<int>(text.size()), 
//    nullptr, 0));
//  
//  // Convert
//  int s = MultiByteToWideChar(CP_ACP, 0, text.c_str(), static_cast<int>(text.size()),
//    const_cast<wchar_t*>(result.data()), static_cast<int>(result.size()));
//  
//  return result;
//}
//
//std::string LocalToUTF8(const std::string & text) {
//  return UTF16ToUTF8(LocalToUTF16(text));
//}

}

