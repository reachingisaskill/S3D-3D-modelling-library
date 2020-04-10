
#ifndef __S3D__LIGHTS_H__
#define __S3D__LIGHTS_H__

#include "S3D_light_base.h"

namespace S3D
{

////////////////////////////////////////////////////////////////////////////////////////////////////
  // Point Source

  class light_pointSource : public object_base
  {
    private:
    protected:
    public:
      // Position and brightness
      light_pointSource( spectrum, double );

      virtual double getSurfaceArea() const { return 0.0; }

      // Pure virtual functions to handle intersection, containment, collision, etc.
      virtual bool contains( const point& ) const { return false; }
      // Strictly only if the distance from the start of the line is positive!!!
      virtual bool crosses( const line& ) const { return false; }
      virtual interaction intersect( const line& ) const { return interaction(); }

      // Returns a random point on the surface of the object,
      virtual surfacemap sampleSurface() const { return surfacemap( this->getPosition(), unit_threeVector_z ); }

      // Randomly samples the light seen at a given point
      //  ray tracer object required to perform visibility checks
      virtual line sampleEmission() const;
  };


////////////////////////////////////////////////////////////////////////////////////////////////////
  // Spot Light

//  class light_spotlight : public object_base
//  {
//    private:
//      double _angularStdDev;
//      double _radius;
//      double _area;
//
//    protected:
//      threeVector _getDirection() const;
//
//    public:
//      // Position, brightness, radius
//      light_spotlight( spectrum, double, double );
//
//      double getSurfaceArea() const { return _area; }
//
//      void setStdDev( unsigned int i ) { _angularStdDev = i; }
//      unsigned int getStdDev() const { return _angularStdDev; }
//
//      // Pure virtual functions to handle intersection, containment, collision, etc.
//      virtual bool contains( const point& ) const { return false; }
//      // Strictly only if the distance from the start of the line is positive!!!
//      virtual bool crosses( const line& ) const { return false; }
//      virtual interaction intersect( const line& ) const { return interaction(); }
//
//      // Returns a random point on the surface of the object,
//      virtual surfacemap sampleSurface() const { return this->getPosition(); }
//
//      // Randomly samples the light seen at a given point
//      //  ray tracer object required to perform visibility checks
//      virtual line sampleEmission() const = 0;
//  };

}

#endif // __S3D__LIGHTS_H__

