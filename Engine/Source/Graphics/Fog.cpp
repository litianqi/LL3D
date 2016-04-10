#include "Graphics/Fog.h"
#include "Graphics/Effects.h"

namespace LL3D {
namespace Graphics {

Fog::Fog(Math::Color color, float start, float range) :
  data_{ color, start, range }
{

}

void Fog::update()
{
  s_effect->setFog(data_);
}

}
}

