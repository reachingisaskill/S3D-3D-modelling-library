
#include "S3D_material_base.h"

#include "S3D_frame.h"


namespace S3D
{

  material_base::material_base() :
    _reflectance( 1.0 ),
    _absorption( 0.0 ),
    _transmittance( 0.0 ),
    _irradiance( 0.0 )
  {
  }
}

