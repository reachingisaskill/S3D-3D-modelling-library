
#ifndef __S3D__CONVERGENCE_H__
#define __S3D__CONVERGENCE_H__

#include "S3D_spectrum.h"


namespace S3D
{
  class convergence_base
  {
    private:
      unsigned int long _counter;
      spectrum _mean;

    protected:
      virtual bool _isConverging( const spectrum& ) = 0;

      virtual void _clear() = 0;

    public:
      convergence_base();

      // Return true until convergence
      bool operator()( const spectrum& );

      unsigned long int getCount() const { return _counter; }

      virtual spectrum getMean() const { return _mean; }

      void clear();
  };


  class convergence_counter : public convergence_base
  {
    private:
      unsigned long int _limit;

    protected:
      virtual bool _isConverging( const spectrum& );
      virtual void _clear();

    public:
      convergence_counter( unsigned long int limit );
  };


  class convergence_error : public convergence_base
  {
    private:
      double _limit;
      double _mean;
      double _sumSquares;
      unsigned int _min;
      unsigned int _max;

    protected:
      virtual bool _isConverging( const spectrum& );
      virtual void _clear();

    public:
      convergence_error( double, unsigned int min = 5 );

      void setMinSamples( unsigned int );
      void setMaxSamples( unsigned int );

      double getMeanIntensity() const { return _mean; }
      double getVariance() const { return _sumSquares / this->getCount(); }
      double getFractionalVariance() const;
      double getError() const;
  };
}

#endif // __S3D__CONVERGENCE_H__

