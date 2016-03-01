#pragma once

#include <filesystem>
#include <map>
#include "Core\Uncopyable.h"

struct ID3D11Device;
struct ID3D11ShaderResourceView;

namespace LL3D {

class Assets : private Uncopyable {
public:
  static void CreateInstance(ID3D11Device* device);
  static Assets* Instance();

  // Get ID3D11ShaderResourceView for specific path.
  // If path can not be found, the function throws an NotFound exception.
  ID3D11ShaderResourceView* GetTexture(std::experimental::filesystem::path path);

private:
  // Load all assets under Content directory.
  Assets(ID3D11Device* device);

  static Assets* s_instance;

  std::map<std::experimental::filesystem::path, ID3D11ShaderResourceView*> textures_cache_;
};

}  // namespace LL3D
