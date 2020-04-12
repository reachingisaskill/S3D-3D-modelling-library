
#ifndef __S3D__MATERIALS_H__
#define __S3D__MATERIALS_H__

#include "S3D_material_base.h"

#include "S3D_vector.h"
#include "S3D_spectrum.h"
#include "S3D_interaction.h"


namespace S3D
{

  class material_simple : public material_base
  {
    private:
      spectrum _colour;
      double _brdf_const;

    protected:
    public:
      material_simple( spectrum );

      virtual double getTransmissionProb( const interaction& ) const { return 0.0; }

      virtual double getReflectionProb( const interaction& ) const { return 0.0; }

      virtual spectrum getColour( surfacemap ) const;

      virtual spectrum BRDF( threeVector, const interaction& ) const;
  };


////////////////////////////////////////////////////////////////////////////////////////////////////
  // Phong Reflection Model

  class material_phong : public material_base
  {
    private:
      spectrum _albedo;
      double _diffuse_coef;
      double _specular_coef;
      double _shininess_factor;

    protected:

    public:
      // Ambient spectrum, Diffuse coef, Specular Coef, Shininess.
      material_phong( spectrum, double, double, double );

      virtual double getTransmissionProb( const interaction& ) const { return 0.0; }

      virtual double getReflectionProb( const interaction& ) const { return 0.0; }

      virtual spectrum getColour( surfacemap ) const { return this->_albedo; }

      virtual spectrum BRDF( threeVector, const interaction& ) const;
  };


////////////////////////////////////////////////////////////////////////////////////////////////////
  // Phong-Blinn Model

  class material_blinn : public material_base
  {
    private:
      spectrum _albedo;
      double _diffuse_coef;
      double _specular_coef;
      double _shininess_factor;

    protected:

    public:
      // Ambient spectrum, Diffuse coef, Specular Coef, Shininess.
      material_blinn( spectrum, double, double, double );

      virtual double getTransmissionProb( const interaction& ) const { return 0.0; }

      virtual double getReflectionProb( const interaction& ) const { return 0.0; }

      virtual spectrum getColour( surfacemap ) const { return this->_albedo; }

      virtual spectrum BRDF( threeVector, const interaction& ) const;
  };


////////////////////////////////////////////////////////////////////////////////////////////////////
  // Perfect Glass Model

  class material_glass : public material_base
  {
    private:
      spectrum _albedo;

    protected:

    public:
      // Colour, Diffuse coef, Specular Coef, Shininess.
      material_glass( spectrum );

      virtual double getTransmissionProb( const interaction& ) const; // Return the fresnel coefficients

      virtual double getReflectionProb( const interaction& ) const; // Return the fresnel coefficients

      virtual spectrum getColour( surfacemap ) const { return this->_albedo; }

      // Use the base class varient - defaults to specular
//      virtual spectrum BRDF( threeVector, const interaction& ) const;
  };


////////////////////////////////////////////////////////////////////////////////////////////////////
  // Perfect Mirror Model

  class material_mirror : public material_base
  {
    private:
      spectrum _albedo;

    protected:

    public:
      // Colour, Diffuse coef, Specular Coef, Shininess.
      material_mirror( spectrum );

      virtual double getTransmissionProb( const interaction& ) const { return 0.0; }

      virtual double getReflectionProb( const interaction& ) const { return 1.0; }

      virtual spectrum getColour( surfacemap ) const { return this->_albedo; }

      // Use the base class varient - defaults to specular
//      virtual spectrum BRDF( threeVector, const interaction& ) const;
  };


////////////////////////////////////////////////////////////////////////////////////////////////////
  // Diffuse Material

  class material_lambertian : public material_base
  {
    private:
      spectrum _albedo;
      double _BRDFConstant;

    protected:

    public:
      material_lambertian( spectrum );

      virtual double getTransmissionProb( const interaction& ) const { return 0.0; }

      virtual double getReflectionProb( const interaction& ) const { return 1.0; }

      virtual spectrum getColour( surfacemap ) const { return this->_albedo; }

      virtual spectrum BRDF( threeVector, const interaction& ) const;

      virtual threeVector sampleReflection( const interaction& ) const;

  };


////////////////////////////////////////////////////////////////////////////////////////////////////
  // Pure Light Source

  class material_lightsource : public material_base
  {
    private:
      spectrum _colour;

    protected:

    public:
      material_lightsource( spectrum, double );

      virtual double getTransmissionProb( const interaction& ) const { return 0.0; }

      virtual double getReflectionProb( const interaction& ) const { return 0.0; }

      virtual spectrum getColour( surfacemap ) const { return this->_colour; }

      virtual spectrum BRDF( threeVector, const interaction& ) const;

      virtual spectrum BTDF( threeVector, const interaction& ) const;

  };


////////////////////////////////////////////////////////////////////////////////////////////////////
  // Glowing Lambertian

  class material_glowing : public material_base
  {
    private:
      spectrum _colour;
      spectrum _albedo;
      double _BRDFConstant;

    protected:

    public:
      // Colour, albedo, emission
      material_glowing( spectrum, spectrum, double );

      virtual double getTransmissionProb( const interaction& ) const { return 0.0; }

      virtual double getReflectionProb( const interaction& ) const { return 1.0; }

      virtual spectrum getColour( surfacemap ) const { return this->_colour; }

      virtual spectrum BRDF( threeVector, const interaction& ) const;

      virtual threeVector sampleReflection( const interaction& ) const;

  };




}

#endif // __S3D__MATERIALS_H__

