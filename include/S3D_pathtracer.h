
#ifndef S3D_PATHTRACER_H_
#define S3D_PATHTRACER_H_

#include "S3D_spectrum.h"
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
      spectrum _pathTrace( point start, threeVector direction, unsigned int depth = 0 );

    public:
      tracer_pathtracer();

      virtual ~tracer_pathtracer();


      // Set hard limit on the number of interactions
      void setMaxDepth( unsigned int d ) { _maxDepth = d; }

      // Monte Carle path end probability
      void setKillProb( double );

      // Interface for camera to trace a single ray
      virtual spectrum traceRay( point start, threeVector direction );

  };

}



#endif // S3D_PATHTRACER_H_

