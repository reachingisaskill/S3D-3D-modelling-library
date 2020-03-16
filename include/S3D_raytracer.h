
#ifndef __S3D__RAYTRACER_H__
#define __S3D__RAYTRACER_H__

#include "S3D_defs.h"
#include "S3D_global.h"

#include "S3D_frame.h"


namespace S3D
{

  class rayTracer
  {
    private:
    protected:
    public:
      rayTracer();

      virtual ~rayTracer();

      virtual beam traceRay( threeVector start, threeVector direction );

  };

}

#endif // __S3D__RAYTRACER_H__

