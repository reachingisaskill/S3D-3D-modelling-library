
#include "S3D_lights.h"

#include "S3D_defs.h"
#include "S3D_frame.h"
#include "S3D_raytracer.h"

#include <cstdlib>
#include <ctime>


namespace S3D
{
    
////////////////////////////////////////////////////////////////////////////////////////////////////
  // Point Source

  light_pointSource::light_pointSource( colour c, double bri ) :
    light_base( c, bri / (4.0*PI) )
  {
  }

  void light_pointSource::sampleRays( const interaction& inter, rayTracer* tracer ) const
  {
    beam b( this->_getColour(), this->_getRadiance() );
    double distance_dimming = 1.0/( 4.0*PI*inter.getDistance()*inter.getDistance() );
    tracer->traceLightSample( b*distance_dimming, this->getPosition(), inter );
  }


////////////////////////////////////////////////////////////////////////////////////////////////////
  // Spot Light

  light_spotlight::light_spotlight( colour c, double bri, double r ) :
    light_base( c, bri / (PI*r*r) ),
    _numSamples( 10 ),
    _angularStdDev( degreesToRadians( 8.0 ) ),
    _radius( std::fabs( r ) )
  {
    srand( time( nullptr ) );
  }


  threeVector light_spotlight::_getDirection() const
  {
    return this->getRotation() * defaultDirection;
  }


  void light_spotlight::sampleRays( const interaction& inter, rayTracer* tracer ) const
  {
    beam b( this->_getColour(), this->_getRadiance() );
    b *= 1.0/_numSamples;
    threeVector direction = _getDirection();
    double gaus_const = 1.0 / ( _angularStdDev * std::sqrt( 2.0 * PI ) );

    for ( unsigned int i = 0; i < _numSamples; ++i )
    {
      double r1 = static_cast<double>( rand() ) / static_cast<double>( RAND_MAX );
      double r2 = static_cast<double>( rand() ) / static_cast<double>( RAND_MAX );

      double radius = _radius * std::sqrt( r1 );
      double theta = r2 * 2.0 * PI;

      double x = radius * std::cos( theta );
      double y = radius * std::sin( theta );

      threeVector pos = makeThreeVector( x, y, 0.0 );

      point start( this->getPosition() + this->getRotation()*pos );
      double angle = std::acos( direction * ( inter.getPoint() - start ).norm() );
      double scale = gaus_const * std::exp( -0.5*std::pow( angle/_angularStdDev, 2 ) ); 

      tracer->traceLightSample( b*scale, start, inter );
    }
  }

}


