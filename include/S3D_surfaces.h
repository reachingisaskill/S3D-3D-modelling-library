
#ifndef S3D_SURFACES_H_
#define S3D_SURFACES_H_

#include "S3D_surface.h"
#include "S3D_defs.h"


namespace S3D
{

  // Rectangular Surface 
  class surface_rectangle : public surface
  {
    private:
      double _widthX;
      double _widthY;

    protected:
    public:
      surface_rectangle();
      surface_rectangle( double, double );
      
      virtual bool crosses( const line& ) const;

      double getLengthX() const { return _widthX; }
      double getLengthY() const { return _widthY; }

      double getArea() const { return _widthX*_widthY; }

      // virtual point intersect( const line& ) const; // Use base class variant

      virtual point sampleSurface() const;
  };


////////////////////////////////////////////////////////////////////////////////////////////////////
  // Circular Surface 

  class surface_circle : public surface
  {
    private:
      double _radius;
      double _radiusSq;

    protected:
    public:
      surface_circle();
      surface_circle( double );
      
      virtual bool crosses( const line& ) const;

      double getRadius() const { return _radius; }

      double getArea() const { return PI*_radius*_radius; }

      // virtual point intersect( const line& ) const; // Use base class variant

      virtual point sampleSurface() const;
  };
}

#endif // S3D_SURFACES_H_

