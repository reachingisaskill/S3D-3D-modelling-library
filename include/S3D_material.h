
#ifndef __S3D_MATERIAL_H__
#define __S3D_MATERIAL_H__

#include "S3D_global.h"

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

      // Relative refractive index
      double _refractiveIndex;

    protected:
    public:
      material_base() {}

      virtual ~texture_base() {}

      // Returns the colour of a point on the container surface, parameterized by a & b.
      virtual colour getColour( double a, double b ) const = 0;
  };
}

#endif // __S3D_MATERIAL_H__

