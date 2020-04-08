
#include "S3D_frame.h"

#include "S3D_defs.h"

#include "stdexts.h"

#include <cmath>


namespace S3D
{

  frame::frame(unsigned int x, unsigned int y ) :
    _pixelsX( x ),
    _pixelsY( y ),
    _image( nullptr )
  {
    _image = new beam[(_pixelsX * _pixelsY)];
  }


  frame::frame( const frame& f ) :
    _pixelsX( f._pixelsX ),
    _pixelsY( f._pixelsY ),
    _image( nullptr )
  {
    _image = new beam[(_pixelsX * _pixelsY)];

    for ( unsigned int i = 0; i < _pixelsX; ++i )
    {
      for ( unsigned int j = 0; j < _pixelsY; ++j )
      {
        this->pixel( i, j ) = f.pixel( i, j );
      }
    }
  }


  frame::frame( frame&& f ) :
    _pixelsX( std::move( f._pixelsX ) ),
    _pixelsY( std::move( f._pixelsY ) ),
    _image( std::exchange( f._image, nullptr ) )
  {
  }


  frame& frame::operator=( const frame& f )
  {
    _pixelsX = f._pixelsX;
    _pixelsY = f._pixelsY;
    if ( this->_image != nullptr )
    {
      delete[] _image;
      _image = new beam[(_pixelsX * _pixelsY)];
    }

    for ( unsigned int i = 0; i < _pixelsX; ++i )
    {
      for ( unsigned int j = 0; j < _pixelsY; ++j )
      {
        this->pixel( i, j ) = f.pixel( i, j );
      }
    }
    
    return *this;
  }


  frame& frame::operator=( frame&& f )
  {
    _pixelsX = std::move( f._pixelsX );
    _pixelsY = std::move( f._pixelsY );

    if ( this->_image != nullptr )
    {
      delete[] _image;
    }
    _image = std::exchange( f._image, nullptr );
    
    return *this;
  }


  frame::~frame()
  {
    if ( this->_image != nullptr )
    {
      delete[] _image;
      _image = nullptr;
    }
  }


  unsigned int frame::_pixelNum( unsigned int x, unsigned int y ) const
  {
    unsigned int n = y*_pixelsX + x;
    return n;
  }


  // Dump to a BMP file appling linear scaling.
  void frame::dump( std::string filename ) const
  {
    stdexts::bitmap bm( this->_pixelsX, this->_pixelsY );

    double maxIntensity = 0.0;
    for ( unsigned int i = 0; i < this->_pixelsX; ++i )
    {
      for ( unsigned int j = 0; j < this->_pixelsY; ++j )
      {
        const beam& b = this->pixel( i, j );
        if ( std::log( 1.0 + b.red() ) > maxIntensity )
          maxIntensity = std::log( 1.0 + b.red() );
        if ( std::log( 1.0 + b.green() ) > maxIntensity )
          maxIntensity = std::log( 1.0 + b.green() );
        if ( std::log( 1.0 + b.blue() ) > maxIntensity )
          maxIntensity = std::log( 1.0 + b.blue() );
      }
    }

    for ( unsigned int i = 0; i < this->_pixelsX; ++i )
    {
      for ( unsigned int j = 0; j < this->_pixelsY; ++j )
      {
        const beam& b = this->pixel( i, j );
        bm.setPixel( i, j, stdexts::bitmap::pixel( std::log( 1.0 + b.red() ) / maxIntensity,
                                                   std::log( 1.0 + b.green() ) / maxIntensity,
                                                   std::log( 1.0 + b.blue() ) / maxIntensity ) );
      }
    }

    bm.saveFile( filename );
  }


//  // Dump to a BMP file appling linear scaling.
//  void frame::dump( std::string filename ) const
//  {
//    stdexts::bitmap bm( this->_pixelsX, this->_pixelsY );
//
//    double maxIntensity = 0.0;
//    for ( unsigned int i = 0; i < this->_pixelsX; ++i )
//    {
//      for ( unsigned int j = 0; j < this->_pixelsY; ++j )
//      {
//        const beam& b = this->pixel( i, j );
//        if ( b.red() > maxIntensity )
//          maxIntensity = b.red();
//        if ( b.green() > maxIntensity )
//          maxIntensity = b.green();
//        if ( b.blue() > maxIntensity )
//          maxIntensity = b.blue();
//      }
//    }
//
//    for ( unsigned int i = 0; i < this->_pixelsX; ++i )
//    {
//      for ( unsigned int j = 0; j < this->_pixelsY; ++j )
//      {
//        const beam& b = this->pixel( i, j );
//        bm.setPixel( i, j, stdexts::bitmap::pixel( b.red()/maxIntensity, b.green()/maxIntensity, b.blue()/maxIntensity ) );
//      }
//    }
//
//    bm.saveFile( filename );
//  }

}

