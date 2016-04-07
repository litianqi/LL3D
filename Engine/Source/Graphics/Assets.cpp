#include "Graphics/Assets.h"
#include <filesystem>
#include <d3d11.h>
#include <DDSTextureLoader.h>
#include "Core/Assert.h"
#include "Core/Exceptions.h"

using namespace std::experimental;
using namespace DirectX;

namespace LL3D {
namespace Graphics {

std::map<std::experimental::filesystem::path, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> s_textures_cache;

ID3D11ShaderResourceView * LoadDDSFromFile(ID3D11Device * device, std::experimental::filesystem::path pathname) {
  // First try to find it in cache.
  auto i = s_textures_cache.find(pathname);
  if (i != s_textures_cache.end()) {
    return i->second.Get();
  }

  // If cannot find it, create it
  Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture_view;
  if (pathname.extension() == ".dds") {
    ThrowIfFailed(
      CreateDDSTextureFromFile(device, pathname.c_str(), nullptr, &texture_view)
      );
  } else {
    throw InvalidArgument("path does not has a extension, or has a extension not supported!");
  }

  s_textures_cache[pathname] = texture_view;
  return texture_view.Get();
}

}  // namespace Graphics
}  // namespace LL3D