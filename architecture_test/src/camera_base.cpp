
#include "S3D_camera_base.h"

#include <utility>

namespace S3D
{

  camera_base::camera_base( threeVector p, rotation r, double f ) :
    base( p, r ),
    _fieldOfView( f ),
    _pixelsX( 800 ),
    _pixelsY( 600 ),
    _frame( nullptr ),
    _rayTracer( new rayTracer() )
  {
  }

  camera_base::camera_base( const camera_base& c ) :
    base( c ),
    _fieldOfView( c._fieldOfView ),
    _pixelsX( c._pixelsX ),
    _pixelsY( c._pixelsY ),
    _frame( nullptr ),
    _rayTracer( new rayTracer( *c._rayTracer ) )
  {
  }

  camera_base::camera_base( camera_base&& c ) noexcept :
    base( std::move( c ) ),
    _fieldOfView( std::move( c._fieldOfView ) ),
    _pixelsX( std::move( c._pixelsX ) ),
    _pixelsY( std::move( c._pixelsY ) ),
    _frame( std::exchange( c._frame, nullptr ) ),
    _rayTracer( std::exchange( c._rayTracer, nullptr ) )
  {
  }

  camera_base& camera_base::operator=( const camera_base& c )
  {
    if ( &c != this )
    {
      base::operator=( c );
      _fieldOfView = c._fieldOfView;
      _pixelsX = c._pixelsX;
      _pixelsY = c._pixelsY;

      if ( _frame != nullptr )
      {
        delete _frame;
        _frame = nullptr;
      }

      if ( _rayTracer != nullptr )
      {
        delete _rayTracer;
      }

      if ( c._rayTracer != nullptr )
      {
        _rayTracer = new rayTracer( *c._rayTracer );
      }
      else 
      {
        _rayTracer = nullptr;
      }
    }

    return *this;
  }

  camera_base& camera_base::operator=( camera_base&& c )
  {
    if ( &c != this )
    {
      base::operator=( std::move( c ) );
      _fieldOfView = std::move( c._fieldOfView );
      _pixelsX = std::move( c._pixelsX );
      _pixelsY = std::move( c._pixelsY );

      if ( _frame != nullptr )
      {
        delete _frame;
      }
      this->_frame = std::exchange( c._frame, nullptr );

      if ( _rayTracer != nullptr )
      {
        delete _rayTracer;
      }
      this->_rayTracer = std::exchange( c._rayTracer, nullptr );

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
    if ( _rayTracer != nullptr )
    {
      delete _rayTracer;
      _rayTracer = nullptr;
    }
  }

  void camera_base::setRayTracer( rayTracer* rt )
  {
    if ( this->_rayTracer != nullptr )
    {
      delete this->_rayTracer;
    }
    this->_rayTracer = rt;
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

