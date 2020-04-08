
#include "S3D_material_base.h"

#include "S3D_frame.h"
#include "S3D_interaction.h"


namespace S3D
{

  material_base::material_base( double N ) :
    _refractiveIndex( N ),
    _emittance( 0.0 )
  {
  }


  threeVector material_base::sampleReflection( const interaction& inter ) const
  {
    return inter.getReflection();
  }


  threeVector material_base::sampleTransmission( const interaction& inter ) const
  {
    return inter.getTransmission();
  }

}

