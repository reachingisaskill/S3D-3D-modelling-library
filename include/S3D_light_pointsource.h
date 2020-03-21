
#ifndef __S3D__LIGHT_POINTSOURCE_H__
#define __S3D__LIGHT_POINTSOURCE_H__

#include "S3D_realVector.h"
#include "S3D_base.h"
#include "S3D_beam.h"
#include "S3D_light_base.h"


namespace S3D
{

  class light_pointSource : public light_interface, public object_base
  {
    private:
    protected:
    public:
      // Position and brightness (total emitted flux)
      light_pointSource( threeVector, double );

      beam calculateFlux( threeVector, const rayTracer* ) const;
  };
}

#endif // __S3D__LIGHT_POINTSOURCE_H__

