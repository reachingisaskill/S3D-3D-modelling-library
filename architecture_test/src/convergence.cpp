
#include "S3D_convergence.h"

#include "S3D_defs.h"


namespace S3D
{

  convergence_base::convergence_base() :
    _counter( 0 ),
    _mean( 0.0, 0.0, 0.0 )
  {
  }


  bool convergence_base::operator()( const spectrum& b )
  {
    ++_counter;
    _mean += ( b - _mean ) * ( 1.0 / _counter );

    return this->_isConverging( b );
  }


  void convergence_base::clear()
  {
    _counter = 0;
    _mean = spectrum( 0.0, 0.0, 0.0 );
    this->_clear();
  }


////////////////////////////////////////////////////////////////////////////////////////////////////

  convergence_counter::convergence_counter( unsigned long int limit ) :
    _limit( limit )
  {
  }


  bool convergence_counter::_isConverging( const spectrum& )
  {
    if ( this->getCount() >= _limit )
      return false;
    else
      return true;
  }


  void convergence_counter::_clear()
  {
  }


////////////////////////////////////////////////////////////////////////////////////////////////////

  convergence_variance::convergence_variance( double limit, unsigned int min ) :
    _limit( limit*limit ), // Square the limit to avoid a sqrt() call
    _mean( 0.0 ),
    _sumSquares( 0.0 ),
    _min( min )
  {
  }


  bool convergence_variance::_isConverging( const spectrum& beam )
  {
//    double val = beam.mod();
    double val = beam.mean();
    unsigned long int count = this->getCount();

    double new_mean = _mean + ( val - _mean ) / count;
    _sumSquares += ( val - _mean ) * ( val - new_mean );

    _mean = new_mean;

    if ( count < _min ) // Make at least "min" measurements
      return true;


    if ( _mean < epsilon ) // Fractional variance is undefined
      return false;

    // (Fractional variance / N) = standard error on the mean squared, assuming gaussian distributed measurements
    double variance = _sumSquares / ( _mean * _mean * count * count );
    if ( variance < _limit )
      return false;
    else
      return true;
  }


  void convergence_variance::setMinSamples( unsigned int i )
  {
    _min = i;
  }


  double convergence_variance::getFractionalVariance() const
  {
    if ( _mean < epsilon ) // Fractional variance is undefined
      return 0.0;
    else
      return _sumSquares / ( _mean * this->getCount() );
  }


  void convergence_variance::_clear()
  {
    _mean = 0.0;
    _sumSquares = 0.0;
  }
}

