#pragma once

#include <string>
#include <Windows.h>

namespace LL3D {

namespace Utils {

// For interface to win32 API functions
std::string Convert(const std::wstring& str, unsigned int code_page = CP_UTF8);
std::wstring Convert(const std::string& str, unsigned int code_page = CP_UTF8);

}  // namespace Utils

}