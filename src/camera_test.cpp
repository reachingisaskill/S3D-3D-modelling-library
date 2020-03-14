
#include "S3D_camera_test.h"

#include "S3D_frame.h"

namespace S3D
{

  camera_test::camera_test( threeVector p, threeVector d, double x, double y ) :
    camera_base( p, d, 0.0 ),
    _width( x ),
    _height( y )
  {
  }

  camera_test::~camera_test()
  {
  }

  void camera_test::shutter()
  {
    frame* f = new frame( _width, _height );
    this->_setFrame( f );
  }

}

