
#include "S3D_point.h"

namespace S3D
{

  point::point( threeVector v ) :
    _position( v )
  {
  }

  point::point( double v ) :
    _position( v )
  {
  }

  point::point( double x, double y, double z ) :
    _position( makeThreeVector( x, y, z ) )
  {
  }

  point::~point()
  {
  }


  point& point::operator= ( threeVector vec )
  {
    this->setPosition( vec );

    return *this;
  }

}

