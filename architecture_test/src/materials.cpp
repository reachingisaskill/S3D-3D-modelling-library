
#include "S3D_materials.h"

#include "S3D_defs.h"

namespace S3D
{


  material_simple::material_simple( colour c ) :
    _colour( c ),
    _brdf_const( 1.0 / (2.0*PI) )
  {
  }


  colour material_simple::getColour( interaction ) const
  {
    return _colour;
  }


  beam material_simple::BRDF( threeVector incomingDir, beam beam_in, const interaction& inter) const
  {
    beam theBeam = beam_in*_colour; // Weight the reflected light by the _colour
    
    theBeam *= -inter.getSurfaceNormal() * incomingDir; // Effect of tilting away from lightsource

    theBeam *= _brdf_const;

    return theBeam;
  }

}

