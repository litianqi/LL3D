#pragma once

#include <string>
#include <Windows.h>

namespace LL3D {
namespace Utils {

// Unicode -> code_page
std::string convert(const std::wstring& text, unsigned int code_page = CP_UTF8);
// code_page -> Unicode
std::wstring convert(const std::string& text, unsigned int code_page = CP_UTF8);

} // namespace Utils
} // namespace LL3D