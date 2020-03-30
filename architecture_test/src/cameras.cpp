
#include "S3D_cameras.h"

#include "S3D_defs.h"
#include "S3D_random.h"


namespace S3D
{

////////////////////////////////////////////////////////////////////////////////////////////////////
  // LINE SCAN CAMERA

  camera_lineScan::camera_lineScan( rayTracer* rt, double x, double y ) :
    camera_base( rt, 0.0 ),
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


////////////////////////////////////////////////////////////////////////////////////////////////////
  // PINHOLE CAMERA

  camera_pinhole::camera_pinhole( rayTracer* rt, double fov ) :
    camera_base( rt, fov )
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

    point beam_start = this->getPosition();
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


////////////////////////////////////////////////////////////////////////////////////////////////////
  // SAMPLING PINHOLE CAMERA

  camera_sampledPinhole::camera_sampledPinhole( rayTracer* rt, double fov, unsigned int sampleRate ) :
    camera_base( rt, fov ),
    _sampleRate( sampleRate )
  {
  }

  camera_sampledPinhole::~camera_sampledPinhole()
  {
  }

  void camera_sampledPinhole::shutter()
  {
    unsigned int pX = this->getPixelsX();
    unsigned int pY = this->getPixelsY();
    frame* f = new frame( pX, pY );

    point beam_start = this->getPosition();
    threeVector camera_direction = this->getRotation().rotateVector( defaultDirection );
    double ratio = std::tan( 0.5*this->getFieldOfView() ) / pX;

    for ( unsigned int i = 0; i < pX; ++i )
    {
      for ( unsigned int j = 0; j < pY; ++j )
      {
        beam b( 0.0, 0.0, 0.0 );
        for ( unsigned int k = 0; k < _sampleRate; ++k )
        {
          double x = (double)i + random::uniformDouble();
          double y = (double)j + random::uniformDouble();

          double phi_x = std::atan(( x - (pX / 2.0) ) * ratio);
          rotation rotX( unit_threeVector_y, phi_x );

          double phi_y = -std::atan(( y - (pY / 2.0) ) * ratio);
          rotation rotY( unit_threeVector_x, phi_y );

          threeVector beam_direction = this->getRotation().rotateVector( rotX( rotY( defaultDirection ) ) );
        
          b += this->_getRayTracer()->traceRay( beam_start, beam_direction );
        }

        f->pixel( i, j ) = b *  ( 1.0 / _sampleRate );
      }
    }
    this->_setFrame( f );
  }

}

