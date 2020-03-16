
#include "S3D_base.h"

#include "S3D_manager.h"


namespace S3D
{

  object_base::object_base( threeVector p, rotation r ) :
    stdexts::non_copyable(),
    _isOwned( false ),
    _colour( S3D_DEFAULT_COLOUR ),
    _layer( 0 ),
    _position( p ),
    _rotation( r )
  {
    // manager::getInstance()->initObject( this );
  }

  object_base::~object_base()
  {
  }

  void object_base::rotate( rotation r )
  {
    this->_rotation *= r;
  }

  void object_base::rotateAbout( rotation r, threeVector p )
  {
    _position = ( r * ( _position - p ) ) + p;
    this->rotate( r );
  }

  /*
  void object_base::print( printer& p ) const
  {
    print_base* newShape = this->_print();

    newShape->setOpacity( _opacity );
    newShape->setColour( _cRed, _cGreen, _cBlue ); 

    p.addShape( newShape );
  }
  */


////////////////////////////////////////////////////////////////////////////////////////////////////
  // Object 3D Base
////////////////////////////////////////////////////////////////////////////////////////////////////

  object_3D_base::object_3D_base( threeVector v, rotation r ) :
    object_base( v, r )
  {
  }

  object_3D_base::~object_3D_base()
  {
  }

}

