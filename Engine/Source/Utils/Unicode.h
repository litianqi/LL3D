#pragma once

#include <string>
#include <Windows.h>

namespace LL3D
{
  namespace Utils
  {
    // Unicode -> code_page 
    std::string Convert(const std::wstring& text, unsigned int code_page = CP_UTF8);
    // code_page -> Unicode 
    std::wstring Convert(const std::string& text, unsigned int code_page = CP_UTF8);

  }  // namespace Utils
}  // namespace LL3D