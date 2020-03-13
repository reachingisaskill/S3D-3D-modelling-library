
#include "S3D_camera_base.h"

namespace S3D
{

  camera_base::camera_base( threeVector p, threeVector d, double f ) :
    _position( p ),
    _direction( d ),
    _fieldOfView( f ),
    _pixelsX( 800 ),
    _pixelsY( 600 ),
    _frame( 0 )
  {
  }

  camera_base::camera_base( const camera_base& c ) :
    _position( c._position ),
    _direction( c._direction ),
    _fieldOfView( c._fieldOfView ),
    _pixelsX( c._pixelsX ),
    _pixelsY( c._pixelsY ),
    _frame( 0 )
  {
  }

  camera_base& camera_base::operator=( const camera_base& c )
  {
    _position = c._position;
    _direction = c._direction;
    _fieldOfView = c._fieldOfView;
    _pixelsX = c._pixelsX;
    _pixelsY = c._pixelsY;

    if ( _frame != 0 )
    {
      delete _frame;
      _frame = 0;
    }

    return *this;
  }

  camera_base::~camera_base()
  {
    if ( _frame != 0 )
    {
      delete _frame;
      _frame = 0;
    }
  }

  void camera_base::setPixels( double x, double y )
  {
    this->_pixelsX = x;
    this->_pixelsY = y;
  }

  stdexts::autoPtr<frame> camera_base::popFrame()
  {
    stdexts::autoPtr<frame> temp( this->_frame );
    if ( this->_frame == 0 )
    {
      return temp;
    }
    else 
    {
      delete this->_frame;
      this->_frame = 0;
      return temp;
    }
  }

}

