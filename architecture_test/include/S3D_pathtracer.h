
#ifndef __S3D__PATHTRACER_H__
#define __S3D__PATHTRACER_H__

#include "S3D_beam.h"
#include "S3D_tracer_base.h"


namespace S3D
{

  // Pather tracer definition
  class tracer_pathtracer : public tracer_base
  {
    private:
      unsigned int _maxDepth;
      double _killProb;

    protected:
      beam _pathTrace( point start, threeVector direction, unsigned int depth = 0 );

    public:
      tracer_pathtracer();

      virtual ~tracer_pathtracer();


      // Set hard limit on the number of interactions
      void setMaxDepth( unsigned int d ) { _maxDepth = d; }

      // Monte Carle path end probability
      void setKillProb( double );

      // Interface for camera to trace a single ray
      virtual beam traceRay( point start, threeVector direction );

  };

}



#endif // __S3D__PATHTRACER_H__

