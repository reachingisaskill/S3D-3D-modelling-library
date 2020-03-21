
#include "S3D_light_pointsource.h"

#include "S3D_frame.h"
#include "S3D_raytracer.h"

namespace S3D
{
    
  light_pointSource::light_pointSource( threeVector pos, double bri ) :
    light_interface( colour( 1.0, 1.0, 1.0 ), bri / (4.0*PI) ),
    object_base( pos, rotation() )
  {
  }

  beam light_pointSource::calculateFlux( threeVector pos, const rayTracer* tracer ) const
  {
    double distance = ( pos - this->getCenter() ).mod();
    if ( tracer->isVisible( this->getCenter(), pos ) )
    {
      double intensity = this->_getRadiance() / (distance*distance);
      beam b( colour(), intensity );
      return b;
    }
    else
      return beam();
  }

}

