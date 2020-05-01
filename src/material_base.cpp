#define LOGTASTIC_DEBUG_OFF

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


  threeVector material_base::sampleEmission( const interaction& inter ) const
  {
    return random::uniformHemisphere( inter.getSurfaceNormal() );
  }


  spectrum material_base::getExitantRadiance( const interaction& inter ) const
  {
    return _emittance * this->getColour( inter.getSurfaceMap() );
  }


  threeVector material_base::sampleReflection( const interaction& inter ) const
  {
    return inter.getReflection();
  }


  threeVector material_base::sampleTransmission( const interaction& inter ) const
  {
    return inter.getTransmission();
  }
  

  double material_base::BRDF( threeVector direction, const interaction& inter ) const
  {
    DEBUG_LOG( "material_base::BRDF() : Only reflection is perfectly specular" );
    DEBUG_STREAM << "   Dot product = " << direction * inter.getReflection();

    // Dot product should always be negative
    if ( ( 1.0 + direction * inter.getReflection() ) < epsilon ) // Basically a delta function...
      return 1.0;
    else
      return 0.0;
  }


  double material_base::BTDF( threeVector direction, const interaction& inter ) const
  {
    DEBUG_LOG( "material_base::BTDF() : Only transmission is perfectly specular" );
    DEBUG_STREAM << "   Dot product = " << direction * inter.getReflection();

    // Dot product should always be positive
    if ( ( 1.0 - direction * inter.getTransmission() ) < epsilon ) // Basically a delta function...
      return 1.0;
    else
      return 0.0;
  }


  spectrum material_base::scatter( threeVector direction, const interaction& inter ) const
  {
    spectrum result = this->getColour( inter.getSurfaceMap() ) * this->BRDF( direction, inter );
    return result;
  }

}

