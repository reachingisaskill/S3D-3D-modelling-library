
#include "S3D_material_simple.h"

#include "S3D_frame.h"

#include <cmath>

namespace S3D
{

  material_simple::material_simple( colour c ) :
    _colour( c )
  {
  }

  beam material_simple::interact( beam b, double angle, const rayTracer* tracer ) const
  {
    b *= std::cos( angle );
    beam reflected = b * this->getColour( 0.0, 0.0 ) * this->getReflectance();
    return reflected;
  }

}

