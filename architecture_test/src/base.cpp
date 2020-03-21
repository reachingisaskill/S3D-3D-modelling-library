
#include "S3D_base.h"


namespace S3D
{

  base::base( point p, rotation r ) :
    stdexts::non_copyable(),
    _isOwned( false ),
    _layer( 0 ),
    _position( p ),
    _rotation( r )
  {
  }

  base::~base()
  {
  }

  void base::rotate( rotation r )
  {
    this->_rotation *= r;
  }

  void base::rotateAbout( rotation r, threeVector p )
  {
    _position = ( r * ( _position - p ) ) + p;
    this->rotate( r );
  }


}

