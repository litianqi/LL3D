#pragma once

#include <filesystem>
#include <map>
#include <wrl.h>
#include "Core/Uncopyable.h"

struct ID3D11Device;
struct ID3D11ShaderResourceView;

namespace LL3D {
namespace Graphics {

// Create and return texture, if texture has already be created,  return it
// immediatlly.
// Exception: If path doesn't exists or has wrong extension, an InvalidArgument
//  exception will be raised.
ID3D11ShaderResourceView* loadDDSFromFile(
  ID3D11Device* device, std::experimental::filesystem::path pathname);

} // namespace Graphics
} // namespace LL3D
