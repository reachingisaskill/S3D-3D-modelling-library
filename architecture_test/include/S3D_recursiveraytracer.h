
#ifndef __S3D__RECURSIVERAYTRACER_H__
#define __S3D__RECURSIVERAYTRACER_H__

#include "S3D_vector.h"
#include "S3D_point.h"
#include "S3D_beam.h"
#include "S3D_interaction.h"
#include "S3D_frame.h"
#include "S3D_tracer_base.h"

namespace S3D
{

  class tracer_recursive : public tracer_base
  {
    private:
      unsigned int _maxDepth;

    protected:
      // Recursive implementation
      virtual beam _traceRay( point start, threeVector direction, unsigned int depth=0 );

    public:
      tracer_recursive();

      virtual ~tracer_recursive();

      void setMaxDepth( unsigned int d ) { _maxDepth = d; }

      // Called by the camera - primary interface to ray tracing
      virtual beam traceRay( point start, threeVector direction );

  };

}

#endif // __S3D__RECURSIVERAYTRACER_H__

