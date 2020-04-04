
#ifndef __S3D__MATERIALS_H__
#define __S3D__MATERIALS_H__

#include "S3D_material_base.h"

#include "S3D_vector.h"
#include "S3D_beam.h"
#include "S3D_interaction.h"


namespace S3D
{

  class material_simple : public material_base
  {
    private:
      colour _colour;
      double _brdf_const;

    protected:
    public:
      material_simple( colour );

      virtual double getTransmissionProb( const interaction& ) const { return 0.0; }

      virtual double getReflectionProb( const interaction& ) const { return 0.0; }

      virtual colour getColour( surfacemap ) const;

      virtual beam BRDF( threeVector, beam, const interaction& ) const;
  };


////////////////////////////////////////////////////////////////////////////////////////////////////
  // Phong Reflection Model

  class material_phong : public material_base
  {
    private:
      colour _ambient_coef;
      double _diffuse_coef;
      double _specular_coef;
      double _shininess_factor;

    protected:

    public:
      // Ambient colour, Diffuse coef, Specular Coef, Shininess.
      material_phong( colour, double, double, double );

      virtual double getTransmissionProb( const interaction& ) const { return 0.0; }

      virtual double getReflectionProb( const interaction& ) const { return 0.0; }

      virtual colour getColour( surfacemap ) const { return this->_ambient_coef; }

      virtual beam BRDF( threeVector, beam, const interaction& ) const;
  };


////////////////////////////////////////////////////////////////////////////////////////////////////
  // Phong-Blinn Model

  class material_blinn : public material_base
  {
    private:
      colour _ambient_coef;
      double _diffuse_coef;
      double _specular_coef;
      double _shininess_factor;

    protected:

    public:
      // Ambient colour, Diffuse coef, Specular Coef, Shininess.
      material_blinn( colour, double, double, double );

      virtual double getTransmissionProb( const interaction& ) const { return 0.0; }

      virtual double getReflectionProb( const interaction& ) const { return 0.0; }

      virtual colour getColour( surfacemap ) const { return this->_ambient_coef; }

      virtual beam BRDF( threeVector, beam, const interaction& ) const;
  };


////////////////////////////////////////////////////////////////////////////////////////////////////
  // Perfect Glass Model

  class material_glass : public material_base
  {
    private:
      colour _colour;

    protected:

    public:
      // Colour, Diffuse coef, Specular Coef, Shininess.
      material_glass( colour );

      virtual double getTransmissionProb( const interaction& ) const; // Return the fresnel coefficients

      virtual double getReflectionProb( const interaction& ) const; // Return the fresnel coefficients

      virtual colour getColour( surfacemap ) const { return this->_colour; }

      virtual beam BRDF( threeVector, beam, const interaction& ) const;
  };


////////////////////////////////////////////////////////////////////////////////////////////////////
  // Perfect Mirror Model

  class material_mirror : public material_base
  {
    private:
      colour _colour;

    protected:

    public:
      // Colour, Diffuse coef, Specular Coef, Shininess.
      material_mirror( colour );

      virtual double getTransmissionProb( const interaction& ) const { return 0.0; }

      virtual double getReflectionProb( const interaction& ) const { return 1.0; }

      virtual colour getColour( surfacemap ) const { return this->_colour; }

      virtual beam BRDF( threeVector, beam, const interaction& ) const;
  };


////////////////////////////////////////////////////////////////////////////////////////////////////
  // Diffuse Material

  class material_lambertian : public material_base
  {
    private:
      colour _albedo;
      double _BRDFConstant;

    protected:

    public:
      material_lambertian( colour );

      virtual double getTransmissionProb( const interaction& ) const { return 0.0; }

      virtual double getReflectionProb( const interaction& ) const { return 1.0; }

      virtual colour getColour( surfacemap ) const { return this->_albedo; }

      virtual beam BRDF( threeVector, beam, const interaction& ) const;

      virtual threeVector sampleReflection( const interaction& ) const;

  };


////////////////////////////////////////////////////////////////////////////////////////////////////
  // Pure Light Source

  class material_lightsource : public material_base
  {
    private:
      colour _colour;

    protected:

    public:
      material_lightsource( colour, double );

      virtual double getTransmissionProb( const interaction& ) const { return 0.0; }

      virtual double getReflectionProb( const interaction& ) const { return 0.0; }

      virtual colour getColour( surfacemap ) const { return this->_colour; }

      virtual beam BRDF( threeVector, beam, const interaction& ) const;

      virtual threeVector sampleReflection( const interaction& ) const;

  };


////////////////////////////////////////////////////////////////////////////////////////////////////
  // Glowing Lambertian

  class material_glowing : public material_base
  {
    private:
      colour _colour;
      double _BRDFConstant;

    protected:

    public:
      material_glowing( colour, double );

      virtual double getTransmissionProb( const interaction& ) const { return 0.0; }

      virtual double getReflectionProb( const interaction& ) const { return 1.0; }

      virtual colour getColour( surfacemap ) const { return this->_colour; }

      virtual beam BRDF( threeVector, beam, const interaction& ) const;

      virtual threeVector sampleReflection( const interaction& ) const;

  };




}

#endif // __S3D__MATERIALS_H__

