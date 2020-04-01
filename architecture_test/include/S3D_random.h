
#ifndef __S3D__RANDOM_H__
#define __S3D__RANDOM_H__

#include "S3D_vector.h"

namespace S3D
{

  namespace random
  {

    void reset();

    double uniformDouble();

    twoVector uniformCircularPlane( double radius = 1.0 );

    twoVector uniformRectangularPlane( double w = 1.0, double l = 1.0 );

    threeVector uniformHemisphere( threeVector normal );
  }
}

#endif // __S3D__RANDOM_H__
