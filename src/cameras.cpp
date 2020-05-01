
#include "S3D_cameras.h"

#include "S3D_defs.h"
#include "S3D_random.h"
#include "S3D_tracer_base.h"

#include "logtastic.h"


namespace S3D
{

////////////////////////////////////////////////////////////////////////////////////////////////////
  // LINE SCAN CAMERA

  camera_lineScan::camera_lineScan( tracer_base* rt, double x, double y ) :
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
    this->_getRayTracer()->setup();
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
      
        spectrum b = this->_getRayTracer()->traceRay( beam_start, beam_direction );

        f->pixel( i, j ) = b;
      }

      INFO_STREAM << "Calculating Pixel Values : " << (100.0*double(i)/double(pX)) << "% Complete";
    }
    this->_setFrame( f );
  }


////////////////////////////////////////////////////////////////////////////////////////////////////
  // PINHOLE CAMERA

  camera_pinhole::camera_pinhole( tracer_base* rt, double fov ) :
    camera_base( rt, fov )
  {
  }

  camera_pinhole::~camera_pinhole()
  {
  }

  void camera_pinhole::shutter()
  {
    this->_getRayTracer()->setup();
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

      
        spectrum b = this->_getRayTracer()->traceRay( beam_start, beam_direction );

        f->pixel( i, j ) = b;
      }

      INFO_STREAM << "Calculating Pixel Values : " << (100.0*double(i)/double(pX)) << "% Complete";
    }
    this->_setFrame( f );
  }


////////////////////////////////////////////////////////////////////////////////////////////////////
  // SAMPLING PINHOLE CAMERA

  camera_sampledPinhole::camera_sampledPinhole( tracer_base* rt, double fov, unsigned int sampleRate ) :
    camera_base( rt, fov ),
    _sampleRate( sampleRate )
  {
  }

  camera_sampledPinhole::~camera_sampledPinhole()
  {
  }

  void camera_sampledPinhole::shutter()
  {
    this->_getRayTracer()->setup();
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
        spectrum b( 0.0, 0.0, 0.0 );
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

      INFO_STREAM << "Calculating Pixel Values : " << (100.0*double(i)/double(pX)) << "% Complete";
    }
    this->_setFrame( f );
  }



////////////////////////////////////////////////////////////////////////////////////////////////////
  // Perspective Camera


  camera_perspective::camera_perspective( tracer_base* rt, double fov, double width, double height ) :
    camera_base( rt, fov ),
    _width( width ),
    _height( height ),
    _sampleRate( 1.0 )
  {
  }

  camera_perspective::~camera_perspective()
  {
  }

  void camera_perspective::shutter()
  {
    this->_getRayTracer()->setup();
    unsigned int pX = this->getPixelsX();
    unsigned int pY = this->getPixelsY();
    frame* f = new frame( pX, pY );

    // Position marks the centre of image plane (in front of the focal point!)
    point camera_start = this->getPosition();
    threeVector camera_direction = this->getRotation().rotateVector( defaultDirection );
    double fov = 0.5*this->getFieldOfView();

    for ( unsigned int i = 0; i < pX; ++i )
    {
      for ( unsigned int j = 0; j < pY; ++j )
      {
        spectrum b( 0.0, 0.0, 0.0 );
        for ( unsigned int k = 0; k < _sampleRate; ++k )
        {
          double pixel_x = (double)i + random::uniformDouble();
          double pixel_y = (double)j + random::uniformDouble();

          threeVector startPos = makeThreeVector( ( ( pixel_x - 0.5*pX ) / pX ) * _width, ( ( pixel_y - 0.5*pY ) / pY ) * _height, 0.0 );

          double x = ( ( pixel_x - 0.5*pX ) / pX ) * _width * ( 1.0 + std::tan( fov ) );
          double y = ( ( pixel_y - 0.5*pY ) / pX ) * _height * ( 1.0 + std::tan( (pY/pX)*fov ) );

          threeVector secondPos = makeThreeVector( x, y, 1.0 );

          point beam_start = camera_start + this->getRotation().rotateVector( startPos );
          point beam_next = camera_start + this->getRotation().rotateVector( secondPos );
          threeVector beam_direction = beam_next - beam_start;
        
          b += this->_getRayTracer()->traceRay( beam_start, beam_direction );
        }

        f->pixel( i, j ) = b *  ( 1.0 / _sampleRate );
      }

      INFO_STREAM << "Calculating Pixel Values : " << (100.0*double(i)/double(pX)) << "% Complete";
    }
    this->_setFrame( f );
  }

////////////////////////////////////////////////////////////////////////////////////////////////////
  // Converging Perspective Camera


  camera_convergePerspective::camera_convergePerspective( tracer_base* rt, double fov, double width, double height, double c ) :
    camera_base( rt, fov ),
    _width( width ),
    _height( height ),
    _converge( c ),
    _hX( 2 ),
    _hY( 3 )
  {
  }

  camera_convergePerspective::~camera_convergePerspective()
  {
  }

  void camera_convergePerspective::shutter()
  {
    this->_getRayTracer()->setup();
    unsigned int pX = this->getPixelsX();
    unsigned int pY = this->getPixelsY();
    frame* f = new frame( pX, pY );

    // Position marks the centre of image plane (in front of the focal point!)
    point camera_start = this->getPosition();
    threeVector camera_direction = this->getRotation().rotateVector( defaultDirection );
    double fov = 0.5*this->getFieldOfView();

    for ( unsigned int i = 0; i < pX; ++i )
    {
      for ( unsigned int j = 0; j < pY; ++j )
      {
        spectrum b;
        _converge.clear();
        do
        {
          double pixel_x = (double)i + _hX.sample();
          double pixel_y = (double)j + _hY.sample();

          threeVector startPos = makeThreeVector( ( ( pixel_x - 0.5*pX ) / pX ) * _width, ( ( pixel_y - 0.5*pY ) / pY ) * _height, 0.0 );

          double x = ( ( pixel_x - 0.5*pX ) / pX ) * _width * ( 1.0 + std::tan( fov ) );
          double y = ( ( pixel_y - 0.5*pY ) / pX ) * _height * ( 1.0 + std::tan( (pY/pX)*fov ) );

          threeVector secondPos = makeThreeVector( x, y, 1.0 );

          point beam_start = camera_start + this->getRotation().rotateVector( startPos );
          point beam_next = camera_start + this->getRotation().rotateVector( secondPos );
          threeVector beam_direction = beam_next - beam_start;
        
          b = this->_getRayTracer()->traceRay( beam_start, beam_direction );
        }
        while ( _converge( b ) );

        f->pixel( i, j ) = _converge.getMean();
      }

      INFO_STREAM << "Calculating Pixel Values : " << (100.0*double(i)/double(pX)) << "% Complete";
    }
    this->_setFrame( f );
  }
}

