
#include "S3D_frame.h"

namespace S3D
{

  beam::beam() :
    red( 0.0 ),
    green( 0.0 ),
    blue( 0.0 ),
    numReflections( 0 )
  {
  }



  frame::frame(unsigned int x, unsigned int y ) :
    _pixelsX( x ),
    _pixelsY( y ),
    _image( 0 )
  {
    _image = new beam[(_pixelsX * _pixelsY)];
  }


  frame::frame( const frame& f ) :
    _pixelsX( f._pixelsX ),
    _pixelsY( f._pixelsY ),
    _image( 0 )
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


  frame& frame::operator=( const frame& f )
  {
    _pixelsX = f._pixelsX;
    _pixelsY = f._pixelsY;
    if ( this->_image != 0 )
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


  frame::~frame()
  {
    if ( this->_image != 0 )
    {
      delete[] _image;
      _image = 0;
    }
  }


  unsigned int frame::_pixelNum( unsigned int x, unsigned int y ) const
  {
    unsigned int n = y*_pixelsX + x;
    return n;
  }


  void frame::dump( std::string filename ) const
  {
    stdexts::bitmap bm( this->_pixelsX, this->_pixelsY );

    for ( unsigned int i = 0; i < this->_pixelsX; ++i )
    {
      for ( unsigned int j = 0; j < this->_pixelsY; ++j )
      {
        const beam& r = this->pixel( i, j );
        bm.setPixel( i, j, stdexts::bitmap::pixel(r.red, r.green, r.blue) );
      }
    }

    bm.saveFile( filename );
  }

}

