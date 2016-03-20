#include "Fog.h"
#include "Effects.h"

namespace LL3D {
namespace Graphics {

Fog::Fog(Math::Color color, float start, float range) :
  data_{ color, start, range }
{

}

std::unique_ptr<Component> Fog::Clone()
{
  return std::make_unique<Fog>(*this);
}

void Fog::Update()
{
  s_effect->SetFog(data_);
}

}
}

