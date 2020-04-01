
#ifndef __S3D__LIGHT_BASE_H__
#define __S3D__LIGHT_BASE_H__

#include "S3D_vector.h"
#include "S3D_rotation.h"
#include "S3D_base.h"
#include "S3D_beam.h"
#include "S3D_interaction.h"

namespace S3D
{
  class tracer_base;

  class light_base : public base
  {
    private:
      colour _colour;
      double _radiance;

    protected:
      // Colour and radiance (flux per unit area, per solid angle)
      light_base( colour, double );

      const double& _getRadiance() const { return _radiance; }
      const colour& _getColour() const { return _colour; }

    public:
      // Call the light tracing routines, sampling the emitted rays.
      virtual beam sampleRays( const interaction&, const tracer_base* ) const = 0;

  };
}

#endif // __S3D__LIGHT_BASE_H__

