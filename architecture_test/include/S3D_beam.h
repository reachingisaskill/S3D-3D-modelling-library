
#ifndef __S3D__BEAM_H__
#define __S3D__BEAM_H__


namespace S3D
{

  class colour
  {
    private:
      // Floating point pixel values [0,1]
      double _red;
      double _green;
      double _blue;

    public :
      colour();
      colour( double, double, double );

      const double& getRed() const { return _red; }
      const double& getGreen() const { return _green; }
      const double& getBlue() const { return _blue; }

      // Ensure all components are between [0,1]
      void setRed( double );
      void setGreen( double );
      void setBlue( double );


      bool operator==( const colour& ) const;
      bool operator!=( const colour& c ) const { return ! colour::operator==( c ); }
      colour operator*( double ) const;
  };


  /*
    Technically we're using the RGB colour class to implement a 3-point
    sampled power distribution. It can be initialised with a colour and an
    intensity, or the sampled powers for each colour.
    Also stores useful information from the ray tracing algorithm.
  */
  class beam
  {
    private:
      // Spectral flux values
      double _red;
      double _green;
      double _blue;
      unsigned int _numReflections;

    protected:
      beam( double, double, double, unsigned int );

    public:
      beam();

      // Initialise with the spectral fluxes
      beam( double, double, double );

      // Initialise with a colour and an intensity
      beam( colour, double );

      void reflect() { _numReflections += 1; }

      const double& red() const { return this->_red; }
      const double& green() const { return this->_green; }
      const double& blue() const { return this->_blue; }


      beam operator+( const beam& ) const;
      beam operator*( double ) const;
      beam operator*( const colour& ) const;
      beam operator*( const beam& ) const;
      beam& operator+=( const beam& );
      beam& operator*=( double );
    
      friend beam operator*( double, const beam& );
  };


  beam operator*( double d, const beam& b );
}

#endif // __S3D__BEAM_H__

