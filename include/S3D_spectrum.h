
#ifndef S3D_BEAM_H_
#define S3D_BEAM_H_

#include <cmath>


namespace S3D
{

  /*
    Technically we're using the RGB spectrum class to implement a 3-point
    sampled power distribution.
  */
  class spectrum
  {
    private:
      // Spectral power density values
      double _red;
      double _green;
      double _blue;

    protected:

    public:
      spectrum();

      // Initialise uniform distribution
      explicit spectrum( double );

      // Initialise with the spectral fluxes
      spectrum( double, double, double );

      double red() const { return this->_red; }
      double green() const { return this->_green; }
      double blue() const { return this->_blue; }

      void setRed( double r ) { _red = r; }
      void setGreen( double g ) { _green = g; }
      void setBlue( double b ) { _blue = b; }

      double square() const { return _red*_red + _green*_green + _blue*_blue; }
      double mod() const { return std::sqrt(_red*_red + _green*_green + _blue*_blue); }
      double mean() const { return ( _red + _green + _blue ) * (1.0/3.0); }

      spectrum operator+( const spectrum& ) const;
      spectrum operator-( const spectrum& ) const;
      spectrum operator*( double ) const;
      spectrum operator*( const spectrum& ) const;
      spectrum& operator+=( const spectrum& );
      spectrum& operator*=( double );
    
      friend spectrum operator*( double, const spectrum& );
  };


  spectrum operator*( double d, const spectrum& b );
}

#endif // S3D_BEAM_H_

