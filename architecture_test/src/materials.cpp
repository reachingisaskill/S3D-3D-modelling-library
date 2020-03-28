#define __DEBUG_OFF__

#include "S3D_materials.h"

#include "S3D_defs.h"
#include "S3D_manager.h"

#include "logtastic.h"


namespace S3D
{


  material_simple::material_simple( colour c ) :
    _colour( c ),
    _brdf_const( 1.0 )
  {
  }


  colour material_simple::getColour( const interaction& ) const
  {
    return _colour;
  }


  beam material_simple::scatter( threeVector incomingDir, beam beam_in, const interaction& inter) const
  {
    beam theBeam = beam_in*_colour; // Weight the reflected light by the _colour
    
    theBeam *= -inter.getSurfaceNormal() * incomingDir; // Effect of tilting away from lightsource

    theBeam *= _brdf_const;

    return theBeam;
  }

////////////////////////////////////////////////////////////////////////////////////////////////////
  // Phong Reflection Model

  material_phong::material_phong( colour amb, double dif, double spec, double shin ) :
    _ambient_coef( amb ),
    _diffuse_coef( dif ),
    _specular_coef( spec ),
    _shininess_factor( shin )
  {
  }


  beam material_phong::scatter( threeVector incomingDir, beam beam_in, const interaction& inter ) const
  {
    double L_dot_N = incomingDir * inter.getSurfaceNormal();
    threeVector R = incomingDir - 2.0*inter.getSurfaceNormal() * L_dot_N;
    DEBUG_STREAM << "Incoming = " << incomingDir <<  " -- L dot N = " << L_dot_N << " --  R = " << R;

    // Ambient light added by the raytracer as it varies with sampling frequencies, etc.
//    beam the_beam = manager::getInstance()->getAmbientLight() * _ambient_coef * ( 1.0 / manager::getInstance()->getLightSampleRate() );

    beam the_beam = beam_in *  -L_dot_N * _diffuse_coef * _ambient_coef;
    DEBUG_STREAM << "Plus Diffuse component: " << the_beam.red() << ", " << the_beam.green() << ", " << the_beam.blue();

    double specular_product = -inter.getLine().getDirection() * R;
    if ( specular_product > 0.0 )
    {
      the_beam += beam_in * _specular_coef * std::pow( specular_product, _shininess_factor );
    }
    DEBUG_STREAM << "Plus specular component: " << the_beam.red() << ", " << the_beam.green() << ", " << the_beam.blue();

    return the_beam;
    
  }


}

