
#include "S3D_random.h"

#include "S3D_defs.h"

#include <cmath>
#include <cstdlib>


namespace S3D
{
  namespace random
  {

    void reset()
    {
      srand( time( nullptr ) );
    }


    double uniformDouble()
    {
      double r = static_cast<double>( rand() ) / static_cast<double>( RAND_MAX );
      return r;
    }


    twoVector uniformCircularPlane( double radius )
    {
      double r1 = uniformDouble();
      double r2 = uniformDouble();

      double r = radius * std::sqrt( r1 );
      double theta = r2 * 2.0 * PI;

      double x = r * std::cos( theta );
      double y = r * std::sin( theta );

      return makeTwoVector( x, y );
    }


    twoVector uniformRectangularPlane( double w, double l )
    {
      double r1 = uniformDouble() * w - 0.5 * w;
      double r2 = uniformDouble() * l - 0.5 * l;

      return makeTwoVector( r1, r2 );
    }


    threeVector uniformHemisphere( threeVector normal )
    {
      double r1 = uniformDouble();
      double r2 = uniformDouble();

      double r = std::sqrt( r1 );
      double theta = r2 * 2.0 * PI;

      return normal + makeThreeVector( r * std::cos( theta ), r * std::sin( theta ), r1 );
    }

  }
}

