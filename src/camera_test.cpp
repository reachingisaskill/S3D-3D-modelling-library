
#include "S3D_camera_test.h"

#include "S3D_frame.h"

namespace S3D
{

  camera_test::camera_test( threeVector p, rotation d, double x, double y ) :
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
    frame* f = new frame( this->getPixelsX(), this->getPixelsY() );
    unsigned int pX = this->getPixelsX();
    unsigned int pY = this->getPixelsY();
    for ( unsigned int i = 0; i < pX; ++i )
    {
      for ( unsigned int j = 0; j < pY; ++j )
      {
        f->pixel( i, j ) = beam( (double)i/pX, (double)(pY-j)/pY, 1.0 );
      }
    }
    this->_setFrame( f );
  }
}

