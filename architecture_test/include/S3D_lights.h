
#ifndef __S3D__LIGHTS_H__
#define __S3D__LIGHTS_H__

#include "S3D_light_base.h"

namespace S3D
{

  class light_pointSource : public light_base
  {
    private:
    protected:
    public:
      // Position and brightness
      light_pointSource( colour, double );

      void sampleRays( const interaction&, rayTracer* ) const;
  };
}

#endif // __S3D__LIGHTS_H__

