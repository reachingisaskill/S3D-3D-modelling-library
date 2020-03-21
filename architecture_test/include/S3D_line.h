
#ifndef __S3D__LINE_H__
#define __S3D__LINE_H__

#include "S3D_vector.h"
#include "S3D_rotation.h"
#include "S3D_point.h"


namespace S3D
{

  class line
  {
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

      virtual inline const threeVector& getStart() const { return _position; }
      virtual inline void setStart( threeVector p ) { _position = p; }

      virtual inline const threeVector& getPosition() const { return _position; }
      virtual inline void setPosition( threeVector p ) { _position = p; }

      virtual inline const threeVector& getDirection() const { return _direction; }
      virtual inline void setDirection( threeVector d ) { _direction = d.norm(); }

      virtual threeVector separation( const line& ) const;
      virtual double distance( const line& ) const;

      virtual threeVector separation( const point& ) const;
      virtual double distance( const point& ) const;

      virtual void rotate( rotation );
      virtual void rotateAbout( rotation, threeVector );
  };

}

#endif // __S3D__LINE_H__

