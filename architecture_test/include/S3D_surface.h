
#ifndef __SURFACE_H__
#define __SURFACE_H__


#include "S3D_allLines.h"


namespace S3D
{

  class surface
  {
    private:
      point _position;
      threeVector _normal;
      rotation _rotation;

    protected:
      surface( point, rotation );
//      surface( point, threeVector );

    public:
      virtual ~surface();

      const point& getPosition() const { return _position; }
      void setPosition( const point& p ) { _position = p; }

      const threeVector& getNormal() const { return _normal; }
      void setNormal( const threeVector& );

      const rotation& getRotation() const { return _rotation; }
      void setRotation( const rotation& );


      virtual bool inFront( const point& ) const;

      virtual double distance( const point& ) const = 0;
      virtual double distance( const line& ) const = 0;
      virtual bool crosses( const line& ) const = 0;
      virtual point intersect( const line& ) const = 0;

      virtual void rotate( rotation );
      virtual void rotateAbout( rotation, threeVector );
  };
}

#endif // __SURFACE_H__

