
#include "S3D_camera_pinhole.h"

#include <cmath>

namespace S3D
{

  camera_pinhole::camera_pinhole( threeVector p, rotation d, double fov ) :
    camera_base( p, d, fov )
  {
  }

  camera_pinhole::~camera_pinhole()
  {
  }

  void camera_pinhole::shutter()
  {
    frame* f = new frame( this->getPixelsX(), this->getPixelsY() );
    unsigned int pX = this->getPixelsX();
    unsigned int pY = this->getPixelsY();

    threeVector beam_start = this->getPosition();
    threeVector camera_direction = this->getRotation().rotateVector( defaultDirection );
    double ratio = std::tan( 0.5*this->getFieldOfView() ) / pX;

    for ( unsigned int i = 0; i < pX; ++i )
    {
      double phi_x = std::atan(((double)i - (pX / 2.0)) * ratio);
      rotation rotX( unit_threeVector_y, phi_x );
      for ( unsigned int j = 0; j < pY; ++j )
      {

        double phi_y = -std::atan(((double)j - (pY / 2.0)) * ratio);

        rotation rotY( unit_threeVector_x, phi_y );

        threeVector beam_direction = this->getRotation().rotateVector( rotX( rotY( defaultDirection ) ) );

      
        beam b = this->_getRayTracer()->traceRay( beam_start, beam_direction );

        f->pixel( i, j ) = b;
      }
    }
    this->_setFrame( f );
  }

}


