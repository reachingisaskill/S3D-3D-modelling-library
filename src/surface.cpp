
#include "S3D_surface.h"

namespace S3D
{

  surface::surface( threeVector p, rotation r ) :
    object_base( p, r ),
    _normal( r.rotateVector( unit_threeVector_z ) )
  {
  }

  surface::~surface()
  {
  }

  bool surface::inFront( const threeVector* v ) const
  {
    return ( this->distance( v ) > 0.0 );
  }

  bool surface::InFront( const threeVector* v ) const
  {
    return ( this->distance( v ) >= 0.0 );
  }

  void surface::rotate( rotation r )
  {
    this->_normal = r * this->_normal;
    object_base::rotate( r );
  }

  void surface::rotateAbout( rotation r, threeVector p )
  {
    this->_normal = r * this->_normal;
    object_base::rotateAbout( r, p );
  }

}

