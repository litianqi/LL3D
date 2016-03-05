#pragma once

#include <filesystem>
#include <map>
#include <wrl.h>
#include "Core\Uncopyable.h"

struct ID3D11Device;
struct ID3D11ShaderResourceView;

namespace LL3D {

// Exception: if path can not be resolved or has wrong extension, 
// an InvalidArgument exception will raise.
ID3D11ShaderResourceView* CreateTexture(ID3D11Device* device, std::experimental::filesystem::path path);

}  // namespace LL3D
