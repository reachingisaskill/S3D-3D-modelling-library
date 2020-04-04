
#include "S3D_beam.h"

namespace S3D
{

  colour::colour() :
    _red( 1.0 ),
    _green( 1.0 ),
    _blue( 1.0 )
  {
  }

  colour::colour( double r, double g, double b ) :
    _red( r ),
    _green( g ),
    _blue( b )
  {
    if ( _red > 1.0 ) _red = 1.0;
    if ( _red < 0.0 ) _red = 0.0;

    if ( _green > 1.0 ) _green = 1.0;
    if ( _green < 0.0 ) _green = 0.0;

    if ( _blue > 1.0 ) _blue = 1.0;
    if ( _blue < 0.0 ) _blue = 0.0;
  }

  void colour::setRed( double r ) 
  {
    if ( r > 1.0 ) r = 1.0;
    else if ( r < 0.0 ) r = 0.0;
    _red = r;
  }

  void colour::setGreen( double g ) 
  {
    if ( g > 1.0 ) g = 1.0;
    else if ( g < 0.0 ) g = 0.0;
    _green = g;
  }

  void colour::setBlue( double b ) 
  {
    if ( b > 1.0 ) b = 1.0;
    else if ( b < 0.0 ) b = 0.0;
    _blue = b;
  }

  bool colour::operator==( const colour& col ) const
  {
    if ( this->_red != col._red ) return false;
    if ( this->_green != col._green ) return false;
    if ( this->_blue != col._blue ) return false;
    return true;
  }


  colour colour::operator*( double val ) const
  {
    return colour( _red*val, _green*val, _blue*val );
  }


////////////////////////////////////////////////////////////////////////////////////////////////////
  // BEAM


  beam::beam() :
    _red( 0.0 ),
    _green( 0.0 ),
    _blue( 0.0 ),
    _numReflections( 0 )
  {
  }

  beam::beam( double red, double green, double blue, unsigned int ref ) :
    _red( red ),
    _green( green ),
    _blue( blue ),
    _numReflections( ref )
  {
  }

  beam::beam( double red, double green, double blue ) :
    _red( red ),
    _green( green ),
    _blue( blue ),
    _numReflections( 0 )
  {
  }

  beam::beam( colour c, double i ) :
    _red( c.getRed() ),
    _green( c.getGreen() ),
    _blue( c.getBlue() ),
    _numReflections( 0 )
  {
    _red *= i;
    _green *= i;
    _blue *= i;
  }


  beam beam::operator+( const beam& b ) const
  {
    double red = this->_red + b._red;
    double green = this->_green + b._green;
    double blue = this->_blue + b._blue;
    unsigned int ref = this->_numReflections + b._numReflections;

    return beam( red, green, blue, ref );
  }

  beam beam::operator*( double val ) const
  {
    double red = this->_red * val;
    double green = this->_green * val;
    double blue = this->_blue * val;
    unsigned int ref = this->_numReflections;

    return beam( red, green, blue, ref );
  }

  beam beam::operator*( const colour& c ) const
  {
    double red = this->_red * c.getRed();
    double green = this->_green * c.getGreen();
    double blue = this->_blue * c.getBlue();
    unsigned int ref = this->_numReflections;

    return beam( red, green, blue, ref );
  }

  beam beam::operator*( const beam& c ) const
  {
    double red = this->_red * c.red();
    double green = this->_green * c.green();
    double blue = this->_blue * c.blue();
    unsigned int ref = this->_numReflections + c._numReflections;

    return beam( red, green, blue, ref );
  }

  beam& beam::operator+=( const beam& b )
  {
    this->_red += b._red;
    this->_green += b._green;
    this->_blue += b._blue;
    this->_numReflections += b._numReflections;

    return *this;
  }

  beam& beam::operator*=( double val )
  {
    this->_red *= val;
    this->_green *= val;
    this->_blue *= val;

    return *this;
  }

////////////////////////////////////////////////////////////////////////////////////////////////////
  // Friend Functions

  beam operator*( double d, const beam& b )
  {
    return b.operator*( d );
  }
}

