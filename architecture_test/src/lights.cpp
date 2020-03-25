
#include "S3D_lights.h"

#include "S3D_defs.h"
#include "S3D_frame.h"
#include "S3D_raytracer.h"

namespace S3D
{
    
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

}


