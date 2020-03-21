
#ifndef __S3D_COLOUR_H__
#define __S3D_COLOUR_H__


#include "S3D_defs.h"


namespace S3D
{

  class printer;


  class colour
  {
    friend class printer;

    private:
      double _red;
      double _green;
      double _blue;
      double _opacity;

    public:
      colour();
      colour( double, double, double, double = S3D_DEFAULT_COLOUR_OPAC );

      const double& getOpacity() const { return _opacity; }
      void getOpacity( double opac ) { _opacity = opac; }

      const double& getRed() const { return _red; }
      void setRed( double red ) { _red = red; }

      const double& getGreen() const { return _green; }
      void setGreen( double green ) { _green = green; }

      const double& getBlue() const { return _blue; }
      void setBlue( double blue ) { _blue = blue; }


      bool operator==( const colour& );
      bool operator!=( const colour& c ) { return ! colour::operator==( c ); }
  };

}

#endif // __S3D_COLOUR_H__

