#define LOGTASTIC_DEBUG_OFF

#include "S3D_materials.h"

#include "S3D_defs.h"
#include "S3D_manager.h"
#include "S3D_interaction.h"
#include "S3D_random.h"

#include "logtastic.h"


namespace S3D
{


  material_simple::material_simple( spectrum c ) :
    _colour( c ),
    _brdf_const( 1.0 )
  {
  }


  spectrum material_simple::getColour( surfacemap ) const
  {
    return _colour;
  }


  double material_simple::BRDF( threeVector, const interaction& ) const
  {
    return _brdf_const;
  }


  spectrum material_simple::scatter( threeVector, const interaction& ) const
  {
    return _colour * _brdf_const;
  }


////////////////////////////////////////////////////////////////////////////////////////////////////
  // Phong Reflection Model

  material_phong::material_phong( spectrum amb, double dif, double spec, double shin ) :
    _albedo( amb ),
    _diffuse_coef( dif ),
    _specular_coef( spec ),
    _shininess_factor( shin )
  {
  }


  spectrum material_phong::scatter( threeVector incomingDir, const interaction& inter ) const
  {
    double L_dot_N = incomingDir * inter.getSurfaceNormal();
    threeVector R = incomingDir - 2.0*inter.getSurfaceNormal() * L_dot_N;
    DEBUG_STREAM << "PHONG Incoming = " << incomingDir <<  " -- L dot N = " << L_dot_N << " --  R = " << R;

    spectrum coefficient = _diffuse_coef * _albedo * -L_dot_N;
    DEBUG_STREAM << "Plus Diffuse component: " << coefficient.red() << ", " << coefficient.green() << ", " << coefficient.blue();

    double specular_product = -inter.getLine().getDirection() * R;
    if ( specular_product > 0.0 )
    {
      coefficient += spectrum( 1.0, 1.0, 1.0 ) * _specular_coef * std::pow( specular_product, _shininess_factor );
    }
    DEBUG_STREAM << "Plus specular component: " << coefficient.red() << ", " << coefficient.green() << ", " << coefficient.blue();

    return coefficient;
  }


////////////////////////////////////////////////////////////////////////////////////////////////////
  // Phong-Blinn Model

  material_blinn::material_blinn( spectrum amb, double dif, double spec, double shin ) :
    _albedo( amb ),
    _diffuse_coef( dif ),
    _specular_coef( spec ),
    _shininess_factor( shin )
  {
  }


  spectrum material_blinn::scatter( threeVector incomingDir, const interaction& inter ) const
  {
    double L_dot_N = incomingDir * inter.getSurfaceNormal();
    threeVector H = ( -inter.getLine().getDirection() - incomingDir ).norm();
    DEBUG_STREAM << "BLINN Incoming = " << incomingDir <<  " -- L dot N = " << L_dot_N << " --  H = " << H;

    // Ambient light added by the raytracer as it varies with sampling frequencies, etc.

    spectrum coefficient = _diffuse_coef * _albedo * -L_dot_N;
    DEBUG_STREAM << "Plus Diffuse component: " << coefficient.red() << ", " << coefficient.green() << ", " << coefficient.blue();

    double specular_product = inter.getSurfaceNormal() * H;
    if ( specular_product > 0.0 )
    {
      coefficient += spectrum( 1.0, 1.0, 1.0 ) * _specular_coef * std::pow( specular_product, _shininess_factor );
    }
    DEBUG_STREAM << "Plus specular component: " << coefficient.red() << ", " << coefficient.green() << ", " << coefficient.blue();

    return coefficient;
  }


////////////////////////////////////////////////////////////////////////////////////////////////////
  // Glass Model

  material_glass::material_glass( spectrum col ) :
    _albedo( col )
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


////////////////////////////////////////////////////////////////////////////////////////////////////
  // Mirror Model

  material_mirror::material_mirror( spectrum col ) :
    _albedo( col )
  {
  }


////////////////////////////////////////////////////////////////////////////////////////////////////
  // Lambertian Model

  material_lambertian::material_lambertian( spectrum col ) :
    _albedo( col ),
    _BRDFConstant( 1.0 / PI )
  {
  }


  double material_lambertian::BRDF( threeVector, const interaction& ) const
  {
    DEBUG_LOG( "Lambertian scattering" );
    return _BRDFConstant;
  }


  threeVector material_lambertian::sampleReflection( const interaction& inter ) const
  {
    return random::uniformHemisphere( inter.getSurfaceNormal() );
  }


////////////////////////////////////////////////////////////////////////////////////////////////////
  // Pure Light Source

  material_lightsource::material_lightsource( spectrum col, double e ) :
    _colour( col )
  {
    this->setEmittance( e );
  }


  double material_lightsource::BRDF( threeVector, const interaction& ) const
  {
    DEBUG_LOG( "Perfect light source acting as a perfect black." );
    return 0.0;
  }


  double material_lightsource::BTDF( threeVector, const interaction& ) const
  {
    DEBUG_LOG( "Perfect light source acting as a perfect black." );
    return 0.0;
  }


////////////////////////////////////////////////////////////////////////////////////////////////////
  // Glowing lambertian

  material_glowing::material_glowing( spectrum col, spectrum albedo, double e ) :
    _colour( col ),
    _albedo( albedo ),
    _BRDFConstant( 1.0 / PI )
  {
    this->setEmittance( e );
  }


  double material_glowing::BRDF( threeVector, const interaction& ) const
  {
    DEBUG_LOG( "Glowing Lambertian scattering" );
    return _BRDFConstant;
  }


  threeVector material_glowing::sampleReflection( const interaction& inter ) const
  {
    return random::uniformHemisphere( inter.getSurfaceNormal() );
  }
}

