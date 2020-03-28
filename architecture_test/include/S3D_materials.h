
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

      virtual colour getColour( const interaction& ) const;

      virtual beam scatter( threeVector, beam, const interaction& ) const;
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

      virtual colour getColour( const interaction& ) const { return this->_ambient_coef; }

      virtual beam scatter( threeVector, beam, const interaction& ) const;
  };

}

#endif // __S3D__MATERIALS_H__

