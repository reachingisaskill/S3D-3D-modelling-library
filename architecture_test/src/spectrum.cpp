
#include "S3D_spectrum.h"

namespace S3D
{

  spectrum::spectrum() :
    _red( 0.0 ),
    _green( 0.0 ),
    _blue( 0.0 )
  {
  }


  spectrum::spectrum( double red, double green, double blue ) :
    _red( red ),
    _green( green ),
    _blue( blue )
  {
  }


  spectrum spectrum::operator+( const spectrum& b ) const
  {
    double red = this->_red + b._red;
    double green = this->_green + b._green;
    double blue = this->_blue + b._blue;

    return spectrum( red, green, blue );
  }


  spectrum spectrum::operator-( const spectrum& b ) const
  {
    double red = this->_red - b._red;
    double green = this->_green - b._green;
    double blue = this->_blue - b._blue;

    return spectrum( red, green, blue );
  }


  spectrum spectrum::operator*( double val ) const
  {
    double red = this->_red * val;
    double green = this->_green * val;
    double blue = this->_blue * val;

    return spectrum( red, green, blue );
  }


  spectrum spectrum::operator*( const spectrum& c ) const
  {
    double red = this->_red * c._red;
    double green = this->_green * c._green;
    double blue = this->_blue * c._blue;

    return spectrum( red, green, blue );
  }


  spectrum& spectrum::operator+=( const spectrum& b )
  {
    this->_red += b._red;
    this->_green += b._green;
    this->_blue += b._blue;

    return *this;
  }


  spectrum& spectrum::operator*=( double val )
  {
    this->_red *= val;
    this->_green *= val;
    this->_blue *= val;

    return *this;
  }


////////////////////////////////////////////////////////////////////////////////////////////////////
  // Friend Functions

  spectrum operator*( double d, const spectrum& b )
  {
    return b.operator*( d );
  }
}

