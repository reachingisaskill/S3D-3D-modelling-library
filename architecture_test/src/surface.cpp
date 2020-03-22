
#include "S3D_surface.h"

namespace S3D
{

  surface::surface( point p, rotation r ) :
    _position( p ),
    _normal( r.rotateVector( unit_threeVector_z ) )
    _rotation( r )
  {
  }

  surface::~surface()
  {
  }

  
  void surface::setNormal( const threeVector& v )
  {
    threeVector axis = cross( _normal, v );
    double angle = angle( _normal, v );
    _rotation = rotation( axis.norm(), angle );

    _normal = v;
  }


  void surface::setRotation( const rotation& r )
  {
    _rotation = r;
    _normal = r.rotateVector( unit_threeVector_z );
  }


  bool surface::inFront( const threeVector* v ) const
  {
    return ( this->distance( v ) > 0.0 );
  }


  void surface::rotate( rotation r )
  {
    this->_normal = r * this->_normal;
    this->_rotation = r * this->_rotation;
  }

  void surface::rotateAbout( rotation r, threeVector p )
  {
    _position = ( r * ( _position - p ) ) + p;
    this->rotate( r );
  }

}

