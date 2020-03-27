
#ifndef __S3D__LIGHTS_H__
#define __S3D__LIGHTS_H__

#include "S3D_light_base.h"

namespace S3D
{

////////////////////////////////////////////////////////////////////////////////////////////////////
  // Point Source

  class light_pointSource : public light_base
  {
    private:
    protected:
    public:
      // Position and brightness
      light_pointSource( colour, double );

      void sampleRays( const interaction&, rayTracer* ) const;
  };


////////////////////////////////////////////////////////////////////////////////////////////////////
  // Spot Light

  class light_spotlight : public light_base
  {
    private:
      unsigned int _numSamples;
      double _angularStdDev;
      double _radius;

    protected:
      threeVector _getDirection() const;

    public:
      // Position, brightness, radius
      light_spotlight( colour, double, double );

      void setNumSamples( unsigned int i ) { _numSamples = i; }
      unsigned int getNumSamples() const { return _numSamples; }

      void setStdDev( unsigned int i ) { _angularStdDev = i; }
      unsigned int getStdDev() const { return _angularStdDev; }

      void sampleRays( const interaction&, rayTracer* ) const;
  };

}

#endif // __S3D__LIGHTS_H__

