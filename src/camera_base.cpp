
#include "S3D_camera_base.h"

#include <utility>

namespace S3D
{

  camera_base::camera_base( threeVector p, rotation d, double f ) :
    _position( p ),
    _rotation( d ),
    _fieldOfView( f ),
    _pixelsX( 800 ),
    _pixelsY( 600 ),
    _frame( nullptr )
  {
  }

  camera_base::camera_base( const camera_base& c ) :
    _position( c._position ),
    _rotation( c._rotation ),
    _fieldOfView( c._fieldOfView ),
    _pixelsX( c._pixelsX ),
    _pixelsY( c._pixelsY ),
    _frame( nullptr )
  {
  }

  camera_base::camera_base( camera_base&& c ) noexcept :
    _position( std::move( c._position ) ),
    _rotation( std::move( c._rotation ) ),
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
      _rotation = c._rotation;
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
      _rotation = c._rotation;
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

  void camera_base::setPixels( unsigned int x, unsigned int y )
  {
    this->_pixelsX = x;
    this->_pixelsY = y;
  }

  stdexts::autoPtr<frame> camera_base::popFrame()
  {
    stdexts::autoPtr<frame> temp( this->_frame );
    this->_frame = nullptr;
    return temp;
  }

}

