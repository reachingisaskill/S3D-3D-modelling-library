
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

      virtual colour getColour( interaction ) const;

      virtual beam BRDF( threeVector, beam, const interaction& ) const;
  };
}

#endif // __S3D__MATERIALS_H__

