
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

//  threeVector point::operator+ ( const point& p ) const
//  {
//    return _position + p.getPosition();
//  }


  threeVector point::operator- ( const point& p ) const
  {
    return _position - p.getPosition();
  }

  point point::operator+ ( const threeVector& p ) const
  {
    return point( _position + p );
  }

  bool point::operator== ( const point& p ) const
  {
    if ( _position[0] != p._position[0] )
      return false;
    if ( _position[1] != p._position[1] )
      return false;
    if ( _position[2] != p._position[2] )
      return false;

    return true;
  }


  std::ostream& operator<< ( std::ostream& out, point p )
  {
    out << p._position;
    return out;
  }
}

