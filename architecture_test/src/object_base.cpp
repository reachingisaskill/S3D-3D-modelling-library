
#include "S3D_object.h"


namespace S3D
{

  object_base::object_base( material_base* m, double a, double b, double c ) :
    base(),
    _material( m ),
    _boundA( a ),
    _boundB( b ),
    _boundC( c ),
    _ellipse( 1.0/(a*a), 1.0/(b*b), 1.0/(c*c) )
  {
  }

  bool object_base::_bounded( const point& p ) const
  {
    threeVector separation = p.getPosition() - this->getPosition();
    // Separation in object reference frame
    threeVector refSep = this->getRotation() / separation;
    // Scale vector by ellipsoid components
    return ( ( refSep*_ellipse ).mod() <= 1 );
  }

  object_base::~object_base()
  {
  }
}

