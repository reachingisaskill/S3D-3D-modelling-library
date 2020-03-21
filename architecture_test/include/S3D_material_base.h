
#ifndef __S3D_MATERIAL_H__
#define __S3D_MATERIAL_H__

#include "S3D_beam.h"
#include "S3D_raytracer.h"
#include "S3D_surfacemapping.h"

namespace S3D
{

  class material_base
  {
    private:
      // Total integrated fraction of light reflected
      double _reflectance;
      // Total fraction of light absorbed 
      double _absorption; 
      // Total integrated fraction of light transmitted
      double _transmittance;
      // Amount of light emitted from the surface regardless of other effects
      // Luminosity per unit area
      double _irradiance;

      // Relative refractive index
      double _refractiveIndex;

    protected:

    public:
      material_base();
      material_base( double, double, double, double);

      virtual ~material_base() {}

      // Returns the colour of a point on the container surface, parameterized by the surface mapping object
      virtual colour getColour( const surfaceMapping* ) const = 0;

      // Handle the light-surface interaction
      // current light beam, interaction angle, rayTracer that interacted.
      virtual beam interact( beam, double angle, const rayTracer* tracer ) const = 0;

      // Helper to determine light sources
      bool isLightSource() const { return this->_irradiance > 1.0e-9; }

      // TODO: Limit these values so that energy is conserved
      void setReflectance( double r ) { _reflectance = r; }
      void setAbsorption( double a ) { _absorption = a; }
      void setTransmittance( double t ) { _transmittance = t; }
      // Flux emitted per unit surface area
      void setIrradiance( double i ) { _irradiance = i; }

      double getReflectance() const { return _reflectance; }
      double getAbsorption() const { return _absorption; }
      double getTransmittance() const { return _transmittance; }
      // Flux emitted per unit surface area
      double getIrradiance() const { return _irradiance; }
  };
}

#endif // __S3D_MATERIAL_H__

