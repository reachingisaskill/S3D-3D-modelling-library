
#include "S3D_camera_base.h"

#include "S3D_tracer_base.h"

#include <utility>

namespace S3D
{

  camera_base::camera_base( tracer_base* rt, double f ) :
    base(),
    _fieldOfView( f ),
    _pixelsX( 800 ),
    _pixelsY( 600 ),
    _frame( nullptr ),
    _rayTracer( rt )
  {
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


  void camera_base::setRayTracer( tracer_base* rt )
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

