
#include "S3D_camera_base.h"

#include <utility>

namespace S3D
{

  camera_base::camera_base( threeVector p, threeVector d, double f ) :
    _position( p ),
    _direction( d ),
    _fieldOfView( f ),
    _pixelsX( 800 ),
    _pixelsY( 600 ),
    _frame( nullptr )
  {
  }

  camera_base::camera_base( const camera_base& c ) :
    _position( c._position ),
    _direction( c._direction ),
    _fieldOfView( c._fieldOfView ),
    _pixelsX( c._pixelsX ),
    _pixelsY( c._pixelsY ),
    _frame( nullptr )
  {
  }

  camera_base::camera_base( camera_base&& c ) noexcept :
    _position( std::move( c._position ) ),
    _direction( std::move( c._direction ) ),
    _fieldOfView( std::move( c._fieldOfView ) ),
    _pixelsX( std::move( c._pixelsX ) ),
    _pixelsY( std::move( c._pixelsY ) ),
    _frame( std::exchange( c._frame, nullptr ) )
  {
  }

  camera_base& camera_base::operator=( const camera_base& c )
  {
    if ( &c != this )
    {
      _position = c._position;
      _direction = c._direction;
      _fieldOfView = c._fieldOfView;
      _pixelsX = c._pixelsX;
      _pixelsY = c._pixelsY;

      if ( _frame != nullptr )
      {
        delete _frame;
        _frame = nullptr;
      }
    }

    return *this;
  }

  camera_base& camera_base::operator=( camera_base&& c )
  {
    if ( &c != this )
    {
      _position = c._position;
      _direction = c._direction;
      _fieldOfView = c._fieldOfView;
      _pixelsX = c._pixelsX;
      _pixelsY = c._pixelsY;

      if ( _frame != nullptr )
      {
        delete _frame;
      }
      this->_frame = std::exchange( c._frame, nullptr );
    }

    return *this;
  }

  camera_base::~camera_base()
  {
    if ( _frame != nullptr )
    {
      delete _frame;
      _frame = nullptr;
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
    if ( this->_frame == nullptr )
    {
      return temp;
    }
    else 
    {
      delete this->_frame;
      this->_frame = nullptr;
      return temp;
    }
  }

}
