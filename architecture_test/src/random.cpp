#define __DEBUG_OFF__

#include "S3D_random.h"

#include "S3D_defs.h"

#include "logtastic.h"

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


    int uniformInt( int lower , int upper )
    {
      int r = rand() % ( upper - lower );
      return r + lower;
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

      threeVector direction = makeThreeVector( r * std::cos( theta ), r * std::sin( theta ), r1 );

      double angle = vectorAngle( normal, unit_threeVector_z );

      DEBUG_STREAM << "Direction = " << direction << ", Angle = " << angle;

      if ( angle < epsilon ) // Already pointing the right direction.
      {
        DEBUG_LOG( "Parallel to normal" );
        return direction;
      }
      else if ( ( PI - angle ) < epsilon ) // Pointing in the opposite direction
      {
        DEBUG_LOG( "Anti-parallel to normal" );
        return -direction;
      }
      else
      {
        rotation rot = rotation( crossProduct( normal, unit_threeVector_z ), -angle );
        threeVector result = rot.rotateVector( direction );
        DEBUG_STREAM << "Rotating: " << result;
        return result;
      }
    }


    threeVector uniformHemisphere( rotation direction )
    {
      double r1 = uniformDouble();
      double r2 = uniformDouble();

      double r = std::sqrt( r1 );
      double theta = r2 * 2.0 * PI;

      return direction.rotateVector( makeThreeVector( r * std::cos( theta ), r * std::sin( theta ), r1 ) );
    }

////////////////////////////////////////////////////////////////////////////////////////////////////
    // Halton Sequance

    halton::halton( unsigned int base ) :
      _base( base ),
      _count( 0 ),
      _Nn( 0.0 ),
      _Dn( 1.0 )
    {
    }


    double halton::sample()
    {
      double x = _Dn - _Nn;

//      if ( x == 1.0 )
      if ( (x - 1.0) < epsilon )
      {
        _Nn = 1.0;
        _Dn *= _base;
      }
      else
      {
        double y = _Dn / _base;
        while ( x <= y ) y /= _base;
        _Nn = ( 1.0 + _base ) * y - x;
      }

      return _Nn / _Dn;
    }


    // WARN: Untested
//    double halton::sample()
//    {
//      int i = ++_count;
//
//      double f = 1.0;
//      double r = 0.0;
//
//      while ( i > 0 )
//      {
//        f = f/_base;
//        r = r + f*( i % _base );
//        i = i/_base;
//      }
//
//      return r;
//    }

  }
}

