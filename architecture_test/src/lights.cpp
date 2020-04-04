#define __DEBUG_OFF__

#include "S3D_lights.h"

#include "S3D_defs.h"
#include "S3D_frame.h"
#include "S3D_manager.h"
#include "S3D_tracer_base.h"
#include "S3D_random.h"
#include "S3D_materials.h"

#include "logtastic.h"


namespace S3D
{
    
////////////////////////////////////////////////////////////////////////////////////////////////////
  // Point Source

  light_pointSource::light_pointSource( colour c, double bri ) :
    object_base( new material_lightsource( c, bri ) )
  {
  }


  line light_pointSource::sampleEmission() const
  {
    double r1 = random::uniformDouble();
    double r2 = random::uniformDouble();

    double theta = r1*2.0*PI;
    double phi = std::acos( 2.0* r2 - 1 );

    return line( this->getPosition(), makeThreeVector( std::cos(theta)*std::sin(phi), std::sin(theta)*std::sin(phi), std::cos(phi) ) );
  }


////////////////////////////////////////////////////////////////////////////////////////////////////
  // Spot Light
//
//  light_spotlight::light_spotlight( colour c, double bri, double r ) :
//    light_base( c, bri / (PI*r*r) ),
//    _angularStdDev( degreesToRadians( 8.0 ) ),
//    _radius( std::fabs( r ) ),
//    _area( PI*r*r )
//  {
//    random::reset();
//  }
//
//
//  threeVector light_spotlight::_getDirection() const
//  {
//    return this->getRotation() * defaultDirection;
//  }
//
//
//  beam light_spotlight::sampleRays( const interaction& inter, const tracer_base* tracer ) const
//  {
//    beam b_in( this->_getColour(), this->_getRadiance() );
//    unsigned int numSamples = 1 + manager::getInstance()->getLightSampleRate() / this->getArea();
//    b_in *= 1.0 / numSamples;
//    threeVector direction = _getDirection();
//    double gaus_const = 1.0 / ( _angularStdDev * std::sqrt( 2.0 * PI ) );
//
//    beam b_out( 0.0, 0.0, 0.0 );
//    for ( unsigned int i = 0; i < numSamples; ++i )
//    {
//      twoVector plane_pos = random::uniformCircularPlane( _radius );
//      threeVector pos = makeThreeVector( plane_pos[0], plane_pos[1], 0.0 );
//
//      point start( this->getPosition() + this->getRotation()*pos );
//      double angle = std::acos( direction * ( inter.getPoint() - start ).norm() );
//      double scale = gaus_const * std::exp( -0.5*std::pow( angle/_angularStdDev, 2 ) ); 
//
//      b_out += tracer->traceLightSample( b_in*scale, start, inter );
//    }
//    DEBUG_STREAM << "Sampled Light = " << b_out.red() << ", " << b_out.green() << ", " << b_out.blue();
//    return b_out;
//  }

}

