
#include "S3D_lights.h"

#include "S3D_defs.h"
#include "S3D_frame.h"
#include "S3D_manager.h"
#include "S3D_tracer_base.h"
#include "S3D_random.h"

#include "logtastic.h"


namespace S3D
{
    
////////////////////////////////////////////////////////////////////////////////////////////////////
  // Point Source

  light_pointSource::light_pointSource( colour c, double bri ) :
    light_base( c, bri / (4.0*PI) )
  {
  }

  beam light_pointSource::sampleRays( const interaction& inter, const tracer_base* tracer ) const
  {
    beam b( this->_getColour(), this->_getRadiance() );
    double distance_dimming = 1.0/( 4.0*PI*inter.getDistance()*inter.getDistance() );
    return tracer->traceLightSample( b*distance_dimming, this->getPosition(), inter );
  }


////////////////////////////////////////////////////////////////////////////////////////////////////
  // Spot Light

  light_spotlight::light_spotlight( colour c, double bri, double r ) :
    light_base( c, bri / (PI*r*r) ),
    _angularStdDev( degreesToRadians( 8.0 ) ),
    _radius( std::fabs( r ) ),
    _area( PI*r*r )
  {
    random::reset();
  }


  threeVector light_spotlight::_getDirection() const
  {
    return this->getRotation() * defaultDirection;
  }


  beam light_spotlight::sampleRays( const interaction& inter, const tracer_base* tracer ) const
  {
    beam b_in( this->_getColour(), this->_getRadiance() );
    unsigned int numSamples = 1 + manager::getInstance()->getLightSampleRate() / this->getArea();
    b_in *= 1.0 / numSamples;
    threeVector direction = _getDirection();
    double gaus_const = 1.0 / ( _angularStdDev * std::sqrt( 2.0 * PI ) );

    beam b_out( 0.0, 0.0, 0.0 );
    for ( unsigned int i = 0; i < numSamples; ++i )
    {
      twoVector plane_pos = random::uniformCircularPlane( _radius );
      threeVector pos = makeThreeVector( plane_pos[0], plane_pos[1], 0.0 );

      point start( this->getPosition() + this->getRotation()*pos );
      double angle = std::acos( direction * ( inter.getPoint() - start ).norm() );
      double scale = gaus_const * std::exp( -0.5*std::pow( angle/_angularStdDev, 2 ) ); 

      b_out += tracer->traceLightSample( b_in*scale, start, inter );
    }
    DEBUG_STREAM << "Sampled Light = " << b_out.red() << ", " << b_out.green() << ", " << b_out.blue();
    return b_out;
  }


////////////////////////////////////////////////////////////////////////////////////////////////////
  // Circle Area Light

  light_circle::light_circle( colour c, double bri, double r ) :
    light_base( c, bri / (PI*r*r) ),
    _radius( std::fabs( r ) ),
    _area( PI*r*r )
  {
    srand( time( nullptr ) );
  }


  threeVector light_circle::_getDirection() const
  {
    return this->getRotation() * defaultDirection;
  }


  beam light_circle::sampleRays( const interaction& inter, const tracer_base* tracer ) const
  {
    beam b_in( this->_getColour(), this->_getRadiance() );
    unsigned int numSamples = 1 + manager::getInstance()->getLightSampleRate() / this->getArea();
    b_in *= 1.0 / numSamples;

    beam b_out( 0.0, 0.0, 0.0 );
    for ( unsigned int i = 0; i < numSamples; ++i )
    {
      twoVector plane_pos = random::uniformCircularPlane( _radius );
      threeVector pos = makeThreeVector( plane_pos[0], plane_pos[1], 0.0 );

      point start( this->getPosition() + this->getRotation()*pos );

      b_out += tracer->traceLightSample( b_in, start, inter );
    }
    DEBUG_STREAM << "Sampled Light = " << b_out.red() << ", " << b_out.green() << ", " << b_out.blue();
    return b_out;
  }
}

