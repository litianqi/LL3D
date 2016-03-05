#pragma once

#include <DirectXMath.h>

struct Transform {
  DirectX::XMFLOAT3 position;
  DirectX::XMFLOAT3 rotation;
  DirectX::XMFLOAT3 scale;
};