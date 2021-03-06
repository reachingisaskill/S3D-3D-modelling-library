
#ifndef S3D_LINE_H_
#define S3D_LINE_H_

#include "S3D_vector.h"
#include "S3D_rotation.h"
#include "S3D_point.h"


namespace S3D
{

  class line
  {
    friend point lineIntersection( const line&, const line& );

    private:
      point _position;
      threeVector _direction;

    protected:

      rotation _getRotation( threeVector v );

    public:
      // start point and direction
      line( point, threeVector );
      // Start position and rotation wrt the default vector
      line( point, rotation );
      virtual ~line();

      virtual inline const point& getStart() const { return _position; }
      virtual inline void setStart( point p ) { _position = p; }

      virtual inline const point& getPosition() const { return _position; }
      virtual inline void setPosition( point p ) { _position = p; }

      virtual inline const threeVector& getDirection() const { return _direction; }
      virtual inline void setDirection( threeVector d ) { _direction = d.norm(); }

      virtual threeVector separation( const line& ) const;
      virtual double distance( const line& ) const;

      virtual threeVector separation( const point& ) const;
      virtual double distance( const point& ) const;

      virtual void rotate( rotation );
      virtual void rotateAbout( rotation, threeVector );
  };


  point lineIntersection( const line&, const line& );
}

#endif // S3D_LINE_H_

