
#ifndef S3D_BIDIRECTIONALPATHTRACER_H_
#define S3D_BIDIRECTIONALPATHTRACER_H_

#include "S3D_spectrum.h"
#include "S3D_tracer_base.h"
#include "S3D_path.h"


namespace S3D
{

  // Pather tracer definition
  class tracer_bdpt : public tracer_base
  {
    private:
      unsigned int _maxDepth;
      double _killProb;

      path _lightPath;
      path _cameraPath;

    protected:
      void _additionalSetup();

      void _buildPath( path&, line, spectrum );

//      const object_base* _chooseLight() const;

    public:
      tracer_bdpt();

      virtual ~tracer_bdpt();


      // Set hard limit on the number of interactions
      void setMaxDepth( unsigned int d ) { _maxDepth = d; }

      // Monte Carle path end probability
      void setKillProb( double );

      // Interface for camera to trace a single ray
      virtual spectrum traceRay( point start, threeVector direction );

  };

}



#endif // S3D_BIDIRECTIONALPATHTRACER_H_

