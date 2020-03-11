
#include "S3D_point.h"

namespace S3D
{

  point::point( threeVector v ) :
    object_base( v, rotation() )
  {
  }

  point::point( double v ) :
    object_base( threeVector( v ), rotation() )
  {
  }

  point::point( double x, double y, double z ) :
    object_base( makeThreeVector( x, y, z ), rotation() )
  {
  }

  point::~point()
  {
  }


  point& point::operator= ( threeVector vec )
  {
    this->setCenter( vec );

    return *this;
  }

}

