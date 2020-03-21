
#ifndef __S3D__LIGHT_BASE_H__
#define __S3D__LIGHT_BASE_H__

#include "S3D_beam.h"
#include "S3D_raytracer.h"


namespace S3D
{

  // Define the interface for light sources
  class light_interface
  {
    private:
      colour _colour;
      double _radiance;

    protected:
      // Colour and radiance (flux per unit area, per solid angle)
      light_interface( colour, double );

      double _getRadiance() const { return _radiance; }

    public:

      // Calculate the total flux per unit area received at a point in space
      virtual beam calculateFlux( threeVector, const rayTracer* ) const = 0;
  };
}

#endif // __S3D__LIGHT_BASE_H__

