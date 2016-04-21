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

std::map<std::experimental::filesystem::path,
         Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> s_texturesCache;

ID3D11ShaderResourceView*
loadDDSFromFile(ID3D11Device* device,
                std::experimental::filesystem::path pathname)
{
  if (pathname.empty())
    return nullptr;

  // First try to find it in cache.
  auto i = s_texturesCache.find(pathname);
  if (i != s_texturesCache.end()) {
    return i->second.Get();
  }

  // If cannot find it, create it
  Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> textureView;
  if (pathname.extension() == ".dds") {
    throwIfFailed(CreateDDSTextureFromFile(device, pathname.c_str(), nullptr,
                                           &textureView));
  } else {
    throw InvalidArgument(
      "path does not has a extension, or has a extension not supported!");
  }

  if (textureView.Get())
    s_texturesCache[pathname] = textureView;

  return textureView.Get();
}

} // namespace Graphics
} // namespace LL3D