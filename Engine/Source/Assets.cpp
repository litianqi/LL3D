#include "Assets.h"
#include <filesystem>
#include <d3d11.h>
#include <DDSTextureLoader.h>
#include "Core/Assert.h"
#include "Core/Exceptions.h"

namespace LL3D {

using namespace std::experimental;
using namespace DirectX;

Assets::Assets(ID3D11Device* device) {
  for (auto& p : filesystem::recursive_directory_iterator("Resource/Textures")) {
    ID3D11ShaderResourceView* texture_view;
    if (p.path().extension() == ".dds") {
      ThrowIfFailed(
        CreateDDSTextureFromFile(device, p.path().c_str(), nullptr, &texture_view)
        );
    }
    else {
      continue;
      // TODO: support other texture file types.
    }
    
    textures_cache_[p.path().u8string()] = texture_view;
  }
}

void Assets::CreateInstance(ID3D11Device * device) {
  ASSERT(device);
  ASSERT(!s_instance);
  
  s_instance = new Assets(device);
}

Assets * Assets::Instance() {
  ASSERT(s_instance);

  return s_instance;
}

Assets* Assets::s_instance;

ID3D11ShaderResourceView * Assets::GetTexture(filesystem::path path) {
  auto i = textures_cache_.find(path);
  if (i == textures_cache_.end())
    throw NotFound("Assets::GetTexture " + path.u8string());

  return i->second;
}

}  // namespace LL3D