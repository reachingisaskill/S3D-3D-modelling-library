
#include "S3D_cameras.h"

#include "S3D_defs.h"

namespace S3D
{

  camera_lineScan::camera_lineScan( point p, rotation d, double x, double y ) :
    camera_base( p, d, 0.0 ),
    _width( x ),
    _height( y )
  {
  }

  camera_lineScan::~camera_lineScan()
  {
  }

  void camera_lineScan::shutter()
  {
    frame* f = new frame( this->getPixelsX(), this->getPixelsY() );
    unsigned int pX = this->getPixelsX();
    unsigned int pY = this->getPixelsY();


    for ( unsigned int i = 0; i < pX; ++i )
    {
      for ( unsigned int j = 0; j < pY; ++j )
      {
        double posX = (((double)(i+0.5) / pX) - 0.5) * _width;
        double posY = (((double)(j+0.5) / pY) - 0.5) * _height;

        threeVector beam_direction = this->getRotation().rotateVector( defaultDirection );
        point beam_start = this->getPosition() + this->getRotation().rotateVector( makeThreeVector( posX, posY, 0.0 ) );
      
        beam b = this->_getRayTracer()->traceRay( beam_start, beam_direction );

        f->pixel( i, j ) = b;
      }
    }
    this->_setFrame( f );
  }

}

