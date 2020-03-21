
#include "S3D_colour.h"


namespace S3D
{

////////////////////////////////////////////////////////////////////////////////
//colour Methods
////////////////////////////////////////////////////////////////////////////////

  colour::colour() :
    _red( S3D_DEFAULT_COLOUR_RED ),
    _green( S3D_DEFAULT_COLOUR_GREEN ),
    _blue( S3D_DEFAULT_COLOUR_BLUE ),
    _opacity( S3D_DEFAULT_COLOUR_OPAC )
  {
  }

  colour::colour( double red, double green, double blue, double opac ) :
    _red( red ),
    _green( green ),
    _blue( blue ),
    _opacity( opac )
  {
  }

  bool colour::operator==( const colour& col )
  {
    if ( this->_red != col._red ) return false;
    if ( this->_green != col._green ) return false;
    if ( this->_blue != col._blue ) return false;
    if ( this->_opacity != col._opacity ) return false;
    return true;
  }

}

