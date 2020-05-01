
#ifndef S3D_SURFACE_H_
#define S3D_SURFACE_H_


#include "S3D_vector.h"
#include "S3D_point.h"
#include "S3D_line.h"
#include "S3D_rotation.h"


namespace S3D
{

  // Infinite surface
  class surface
  {
    friend line surfaceIntersection( const surface&, const surface& );
    friend point surfaceIntersection( const surface&, const line& );

    private:
      point _position;
      threeVector _normal;
      rotation _rotation;

    protected:

    public:
      // Default constructor
      surface();

      surface( point, rotation );
//      surface( point, threeVector );

      virtual ~surface();

      virtual const point& getPosition() const { return _position; }
      virtual void setPosition( const point& p ) { _position = p; }

      virtual const threeVector& getNormal() const { return _normal; }
      virtual void setNormal( const threeVector& );

      virtual const rotation& getRotation() const { return _rotation; }
      virtual void setRotation( const rotation& );

      // Total area of the surface (infinite surface set to zero.)
      virtual double getArea() const { return 0.0; }

      virtual bool inFront( const point& ) const;

      // Shortest distance from point to surface
      virtual double distance( const point& ) const;
      // Distance from start of line to surface
      virtual double distance( const line& ) const;

      virtual bool crosses( const line& ) const;
      virtual point intersect( const line& ) const;

      // Returns the X-Y vector of the location of intersect in the plane of the surface.
      virtual twoVector planarIntersect( const line& );

      virtual point sampleSurface() const;

      virtual void rotate( rotation );
      virtual void rotateAbout( rotation, point );
  };


  // Declarations
  line surfaceIntersection( const surface&, const surface& );
  point surfaceIntersection( const surface&, const line& );
}

#endif // S3D_SURFACE_H_

