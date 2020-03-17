
#ifndef __S3D__MATERIAL_SIMPLE_H__
#define __S3D__MATERIAL_SIMPLE_H__

#include "S3D_global.h"

#include "S3D_material_base.h"

namespace S3D
{

  /* 
    Makes some very simple assumption to allow the modelling of 
    non-reflective, opaque materials with simple colours
   */
  class material_simple : public material_base
  {
    private:
      colour _colour;
    protected:
    public:
      material_simple(colour);

      virtual ~material_simple() {}

      // Whole thing is the same colour.
      virtual colour getColour( double, double ) const { return _colour; }

      virtual beam interact( beam, double angle, rayTracer* tracer ) const;
  };
}

#endif // __S3D__MATERIAL_SIMPLE_H__

