
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

      beam sampleRays( const interaction&, const tracer_base* ) const;
  };


////////////////////////////////////////////////////////////////////////////////////////////////////
  // Spot Light

  class light_spotlight : public light_base
  {
    private:
      double _angularStdDev;
      double _radius;
      double _area;

    protected:
      threeVector _getDirection() const;

    public:
      // Position, brightness, radius
      light_spotlight( colour, double, double );

      double getArea() const { return _area; }

      void setStdDev( unsigned int i ) { _angularStdDev = i; }
      unsigned int getStdDev() const { return _angularStdDev; }

      beam sampleRays( const interaction&, const tracer_base* ) const;
  };


////////////////////////////////////////////////////////////////////////////////////////////////////
  // Circular Area Light

  class light_circle : public light_base
  {
    private:
      double _radius;
      double _area;
    protected:
      threeVector _getDirection() const;

    public:
      // Colour, brightness, radius
      light_circle( colour, double, double );

      double getArea() const { return _area; }

      beam sampleRays( const interaction&, const tracer_base* ) const;
  };

}

#endif // __S3D__LIGHTS_H__

