
#ifndef __S3D__BIDIRECTIONALPATHTRACER_H__
#define __S3D__BIDIRECTIONALPATHTRACER_H__

#include "S3D_spectrum.h"
#include "S3D_tracer_base.h"
#include "S3D_path.h"


namespace S3D
{

  // Pather tracer definition
  class tracer_bdpt : public tracer_base
  {
    private:
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


      // Monte Carle path end probability
      void setKillProb( double );

      // Interface for camera to trace a single ray
      virtual spectrum traceRay( point start, threeVector direction );

  };

}



#endif // __S3D__BIDIRECTIONALPATHTRACER_H__

