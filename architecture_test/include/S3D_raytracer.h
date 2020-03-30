
#ifndef __S3D__RAYTRACER_H__
#define __S3D__RAYTRACER_H__

#include "S3D_vector.h"
#include "S3D_point.h"
#include "S3D_beam.h"
#include "S3D_interaction.h"
#include "S3D_frame.h"

namespace S3D
{

  class rayTracer
  {
    private:
      int _layer;
      unsigned int _maxDepth;
//      beam _currentBeam;

    protected:
      // Recursive implementation
      virtual beam _traceRay( point start, threeVector direction, unsigned int depth=0 ) const;

    public:
      rayTracer();

      virtual ~rayTracer();

      void setMaxDepth( unsigned int d ) { _maxDepth = d; }

      // Called by the camera - primary interface to ray tracing
      virtual beam traceRay( point start, threeVector direction ) const;

      virtual beam traceLightSample( beam, point, const interaction& ) const;

      // Can one point directly see anpther?
      virtual bool isVisible( point start, point end ) const;

      // Set the layer to render
      void setLayerNum( int l ) { _layer = l; }
  };

}

#endif // __S3D__RAYTRACER_H__

