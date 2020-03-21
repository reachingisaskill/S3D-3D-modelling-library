
#ifndef __S3D__POINT_H__
#define __S3D__POINT_H__

#include "S3D_vector.h"


namespace S3D
{

  // Specifies a point in three-dimensional space.
  class point
  {
    private:
      threeVector _position;

    public:
      point( threeVector = threeVector( 0.0 ) );
      explicit point( double );
      point( double, double, double );
      virtual ~point();

      virtual const threeVector& getPosition() { return _position; }
      virtual void setPosition( threeVector p ) { _position = p; }

      point& operator= ( threeVector );
  };

}

#endif // __S3D__POINT_H__

