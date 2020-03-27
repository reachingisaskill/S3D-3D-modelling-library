
#ifndef __S3D__MATERIAL_BASE_H__
#define __S3D__MATERIAL_BASE_H__

#include "S3D_base.h"
#include "S3D_beam.h"

#include "stdexts.h"

namespace S3D
{

  class interaction;

  class material_base : public base
  {
    private:
      // Relative refractive index
      double _refractiveIndex;

    protected:

    public:
      material_base( double = 1.0 );

      virtual ~material_base() {}

      double getRefractiveIndex() const { return _refractiveIndex; }

      // Returns the colour of a point on the container surface, parameterized by the surface mapping object
      virtual colour getColour( const interaction& ) const = 0;

      // Handle the light-surface interaction
      // incoming direction, outgoing direction, inteaction details
      virtual beam scatter( threeVector, beam, const interaction& ) const = 0;

  };
}

#endif // __S3D_MATERIAL_BASE_H__

