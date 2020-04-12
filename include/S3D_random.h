
#ifndef __S3D__RANDOM_H__
#define __S3D__RANDOM_H__

#include "S3D_vector.h"
#include "S3D_rotation.h"

namespace S3D
{

  namespace random
  {

    void reset();

    double uniformDouble();

    int uniformInt( int, int );

    twoVector uniformCircularPlane( double radius = 1.0 );

    twoVector uniformRectangularPlane( double w = 1.0, double l = 1.0 );

    threeVector uniformHemisphere( threeVector );

    threeVector uniformHemisphere( rotation );


    class halton
    {
      private:
        unsigned int _base;
        unsigned long int _count;

        double _Nn;
        double _Dn;

      public:
        // Specify the x & y 2D bases.
        halton( unsigned int );

        double sample();

        void reset() { _count = 0; }
    };

  }
}

#endif // __S3D__RANDOM_H__
