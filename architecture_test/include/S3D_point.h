
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

      virtual const threeVector& getPosition() const { return _position; }
      virtual void setPosition( threeVector p ) { _position = p; }

      double& operator[]( int i ) { return _position[i]; }
      const double& operator[]( int i ) const { return _position[i]; }

      point& operator= ( threeVector );
//      threeVector operator+ ( const point& ) const;
      threeVector operator- ( const point& ) const;
      point operator+ ( const threeVector& ) const;

      bool operator==( const point& ) const;

      friend std::ostream& operator<< ( std::ostream&, point );
  };

  std::ostream& operator<< ( std::ostream&, point );
}

#endif // __S3D__POINT_H__

