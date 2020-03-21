
#ifndef __S3D__RAYTRACER_H__
#define __S3D__RAYTRACER_H__

#include "S3D_beam.h"
#include "S3D_vector.h"
#include "S3D_frame.h"

namespace S3D
{

  class rayTracer
  {
    private:
      int _layer;

    protected:
      // Recursive-style raytracing. Starting beam, initial position, direction.
      virtual beam _traceRay( beam, threeVector, threeVector ) const;
    public:
      rayTracer();

      virtual ~rayTracer();

      virtual beam traceRay( threeVector start, threeVector direction ) const;

      // Can one point directly see anpther?
      virtual bool isVisible( threeVector start, threeVector end ) const;

      void setLayerNum( int l ) { _layer = l; }
  };

}

#endif // __S3D__RAYTRACER_H__

