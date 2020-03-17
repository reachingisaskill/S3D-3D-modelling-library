
#include "S3D_material_simple.h"

#include "S3D_frame.h"


namespace S3D
{

  material_simple::material_simple( colour c ) :
    _colour( c )
  {
  }

  beam material_simple::interact( beam b, double angle, rayTracer* tracer ) const
  {
    // 1. Trace rays back to light sources - calculate brightness
    // 2. return colour weighted by the illumination.
  }

}

