
#ifndef S3D_MATERIAL_BASE_H_
#define S3D_MATERIAL_BASE_H_

#include "S3D_base.h"
#include "S3D_spectrum.h"
#include "S3D_defs.h"
#include "S3D_texture.h"
#include "S3D_random.h"

#include "stdexts.h"

namespace S3D
{

  class interaction;

  class material_base : public base
  {
    private:
      // Relative refractive index
      double _refractiveIndex;
      double _emittance;

    protected:

    public:
      material_base( double = 1.0 );

      virtual ~material_base() {}

      double getRefractiveIndex() const { return _refractiveIndex; }
      void setRefractiveIndex( double i ) { _refractiveIndex = i; }


      virtual double getTransmissionProb( const interaction& ) const = 0;

      virtual double getReflectionProb( const interaction& ) const = 0;


      // Returns the albedo spectrum of a point on the container surface, parameterized by some surface mapping object
      virtual spectrum getColour( surfacemap ) const = 0;

      // Returns the exitant radiance of a point on the container surface, in the direction of the camera ray
      virtual spectrum getExitantRadiance( const interaction& ) const;


      // Returns the PDF value for the given intersection. Defaults to uniform hemisphere (1.0/2*PI)
      virtual double getEmission( surfacemap ) const { return 0.5/PI; }

      // Sample the emission PDF at the interaction point. Defaults to uniform hemisphere
      virtual threeVector sampleEmission( const interaction& inter ) const;


      // Handle the light-surface reflection
      //   incoming direction, inteaction details
      virtual double BRDF( threeVector, const interaction& ) const;

      // Sample the BRDF diststribuion and return a direction.
      //   defaults to the perfect specular case.
      virtual threeVector sampleReflection( const interaction& ) const;


      // Handle the light-surface transmission
      //   incoming direction, inteaction details
      virtual double BTDF( threeVector, const interaction& ) const;

      // Sample the BTDF diststribuion and return a direction.
      //   defaults to the perfect specular case.
      virtual threeVector sampleTransmission( const interaction& ) const;


      void setEmittance( double e ) { _emittance = e; }
      double getEmittance() const { return _emittance; }

      bool isLightSource() const { return ( this->_emittance > epsilon ); }


      // To support basic ray tracing, implement a combined scatter function for Phong & Blinn style calculations
      virtual spectrum scatter( threeVector, const interaction& ) const;

  };
}

#endif // _S3D_MATERIAL_BASE_H_

