//#define __DEBUG_OFF__

#include "S3D_materials.h"

#include "S3D_defs.h"
#include "S3D_manager.h"
#include "S3D_interaction.h"
#include "S3D_random.h"

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
    DEBUG_STREAM << "PHONG Incoming = " << incomingDir <<  " -- L dot N = " << L_dot_N << " --  R = " << R;

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

////////////////////////////////////////////////////////////////////////////////////////////////////
  // Phong-Blinn Model

  material_blinn::material_blinn( colour amb, double dif, double spec, double shin ) :
    _ambient_coef( amb ),
    _diffuse_coef( dif ),
    _specular_coef( spec ),
    _shininess_factor( shin )
  {
  }


  beam material_blinn::scatter( threeVector incomingDir, beam beam_in, const interaction& inter ) const
  {
    double L_dot_N = incomingDir * inter.getSurfaceNormal();
    threeVector H = ( -inter.getLine().getDirection() - incomingDir ).norm();
    DEBUG_STREAM << "BLINN Incoming = " << incomingDir <<  " -- L dot N = " << L_dot_N << " --  H = " << H;

    // Ambient light added by the raytracer as it varies with sampling frequencies, etc.

    beam the_beam = beam_in *  -L_dot_N * _diffuse_coef * _ambient_coef;
    DEBUG_STREAM << "Plus Diffuse component: " << the_beam.red() << ", " << the_beam.green() << ", " << the_beam.blue();

    double specular_product = inter.getSurfaceNormal() * H;
    if ( specular_product > 0.0 )
    {
      the_beam += beam_in * _specular_coef * std::pow( specular_product, _shininess_factor );
    }
    DEBUG_STREAM << "Plus specular component: " << the_beam.red() << ", " << the_beam.green() << ", " << the_beam.blue();

    return the_beam;
  }


////////////////////////////////////////////////////////////////////////////////////////////////////
  // Glass Model

  material_glass::material_glass( colour col ) :
    _colour( col )
  {
  }


  double material_glass::getTransmissionProb( const interaction& inter ) const
  {
    return 1.0 - getReflectionProb( inter );
  }


  double material_glass::getReflectionProb( const interaction& inter ) const
  {
    double ratio = inter.getIndexRatio();
    double R_0 =  std::pow( ( ratio - 1 ) / ( ratio + 1 ), 2 );
    double cos_theta = -inter.getLine().getDirection() * inter.getSurfaceNormal();

    return R_0 + ( 1.0 - R_0 ) * std::pow( 1.0 - cos_theta, 5 );
  }


  beam material_glass::scatter( threeVector, beam, const interaction& ) const
  {
    DEBUG_LOG( "Perfect Glass - no additional light" );

    return beam( 0.0, 0.0, 0.0 );
  }


////////////////////////////////////////////////////////////////////////////////////////////////////
  // Mirror Model

  material_mirror::material_mirror( colour col ) :
    _colour( col )
  {
  }


  beam material_mirror::scatter( threeVector, beam, const interaction& ) const
  {
    DEBUG_LOG( "Perfect mirror - np additional light" );

    return beam( 0.0, 0.0, 0.0 );
  }


////////////////////////////////////////////////////////////////////////////////////////////////////
  // Lambertian Model

  material_lambertian::material_lambertian( colour col ) :
    _albedo( col )
  {
  }


  beam material_lambertian::scatter( threeVector incomingDir, beam beam_in, const interaction& inter ) const
  {
    DEBUG_LOG( "Lambertian scattering" );
    double L_dot_N = incomingDir * inter.getSurfaceNormal();

    beam the_beam = beam_in *  -L_dot_N * _albedo;
    DEBUG_STREAM << "Albedo component: " << the_beam.red() << ", " << the_beam.green() << ", " << the_beam.blue();

    return the_beam;
  }


  threeVector material_lambertian::sampleReflection( const interaction& inter ) const
  {
    return random::uniformHemisphere( inter.getSurfaceNormal() );
  }

}

