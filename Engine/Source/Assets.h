#pragma once

#include <filesystem>
#include <map>
#include <wrl.h>
#include "Core\Uncopyable.h"

struct ID3D11Device;
struct ID3D11ShaderResourceView;

namespace LL3D {

//>
// Create and return texture, if texture has already be created,  return 
// immediatlly.
// 
// Exception:
// If path doesn't esists or has wrong extension,
// an InvalidArgument exception will be raised.
//
ID3D11ShaderResourceView* CreateTexture(ID3D11Device* device, std::experimental::filesystem::path pathname);

}  // namespace LL3D
