
#include "S3D_object_base.h"

#include <cmath>


namespace S3D
{

  object_base::object_base( material_base* m ) :
    base(),
    _material( m )
  {
  }


  object_base::~object_base()
  {
  }
}

