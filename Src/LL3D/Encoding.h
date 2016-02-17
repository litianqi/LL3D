#pragma once

#include <string>

namespace LL3D {

std::string UTF16ToUTF8(const std::wstring& text);
std::wstring UTF8ToUTF16(const std::string& text);

std::wstring LocalToUTF16(const std::string& text);

std::string LocalToUTF8(const std::string& text);

}