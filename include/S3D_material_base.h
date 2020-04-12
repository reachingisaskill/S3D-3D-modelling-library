
#ifndef __S3D__MATERIAL_BASE_H__
#define __S3D__MATERIAL_BASE_H__

#include "S3D_base.h"
#include "S3D_spectrum.h"
#include "S3D_defs.h"
#include "S3D_texture.h"

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

      // Returns the coloured emission of a point on the object surface, parameterized by the surface mapping object
      virtual spectrum getEmission( surfacemap map ) const { return spectrum( this->getColour( map ) ) * _emittance; }

      // Handle the light-surface reflection
      //   incoming direction, inteaction details
      virtual spectrum BRDF( threeVector, const interaction& ) const;

      // Handle the light-surface transmission
      //   incoming direction, inteaction details
      virtual spectrum BTDF( threeVector, const interaction& ) const;

      // Sample the BRDF diststribuion and return a direction.
      //   defaults to the perfect specular case.
      virtual threeVector sampleReflection( const interaction& ) const;

      // Sample the BTDF diststribuion and return a direction.
      //   defaults to the perfect specular case.
      virtual threeVector sampleTransmission( const interaction& ) const;


      void setEmittance( double e ) { _emittance = e; }

      bool isLightSource() const { return ( this->_emittance > epsilon ); }

  };
}

#endif // __S3D_MATERIAL_BASE_H__

