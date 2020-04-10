#define __DEBUG_OFF__

#include "S3D_material_base.h"

#include "S3D_frame.h"
#include "S3D_interaction.h"

#include "logtastic.h"


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
  

  spectrum material_base::BRDF( threeVector direction, const interaction& inter ) const
  {
    DEBUG_LOG( "material_base::BRDF() : Only reflection is perfectly specular" );
    DEBUG_STREAM << "   Dot product = " << direction * inter.getReflection();

    // Dot product should always be negative
    if ( ( 1.0 + direction * inter.getReflection() ) < epsilon )
      return spectrum( 1.0, 1.0, 1.0 );
    else
      return spectrum( 0.0, 0.0, 0.0 );
  }


  spectrum material_base::BTDF( threeVector direction, const interaction& inter ) const
  {
    DEBUG_LOG( "material_base::BTDF() : Only transmission is perfectly specular" );
    DEBUG_STREAM << "   Dot product = " << direction * inter.getReflection();

    // Dot product should always be positive
    if ( ( 1.0 - direction * inter.getTransmission() ) < epsilon )
      return spectrum( 1.0, 1.0, 1.0 );
    else
      return spectrum( 0.0, 0.0, 0.0 );
  }

}

