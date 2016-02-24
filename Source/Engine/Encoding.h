#pragma once

#include <string>

namespace LL3D {

// UTF16 <-> UTF8
std::string UTF16ToUTF8(const std::wstring& text);
std::wstring UTF8ToUTF16(const std::string& text);

// UTF16 <-> Local(Or in Windows's terms -- OEM/ANSI)
// TODO: std::string UTF16ToLocal(const std::wstring& text);
std::wstring LocalToUTF16(const std::string& text);

// UTF8 <-> Local(Or in Windows's terms -- OEM/ANSI)
std::string LocalToUTF8(const std::string& text);
//TODO: std::string UTF8ToLocal(const std::string& text);

}