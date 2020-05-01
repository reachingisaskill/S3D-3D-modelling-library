
#ifndef S3D_PRIMITIVES_H_
#define S3D_PRIMITIVES_H_

#include "S3D_point.h"
#include "S3D_line.h"
#include "S3D_rotation.h"
#include "S3D_object_base.h"
#include "S3D_surfaces.h"
#include "S3D_interaction.h"
#include "S3D_material_base.h"

namespace S3D
{

////////////////////////////////////////////////////////////////////////////////////////////////////
  // Sphere

  class sphere : public object_base
  {
    private:
      double _radius;
    protected:
    public:
      sphere( material_base*, double );

      double getRadius() const { return _radius; }
      double getSurfaceArea() const { return 4.0*PI*_radius*_radius; }

      virtual bool contains( const point& p ) const;
      virtual bool crosses( const line& ) const;
      virtual interaction intersect( const line& ) const;

      virtual double distance( const line& ) const;
      virtual double distance( const point& ) const;

      virtual surfacemap sampleSurface() const;
      virtual line sampleEmission() const;
  };


////////////////////////////////////////////////////////////////////////////////////////////////////
  // Box

  class box : public object_base
  {
    private:
      double _lengthX;
      double _lengthY;
      double _lengthZ;
      double _areaSum;

      surface_rectangle _surfaces[6];

    protected:
      void _makeSurfaces();

    public:
      // Lengths in x, y and z in the reference frame of the box
      box( material_base*, double, double, double );

      // Wrapping base class setters to call _makeSurfaces.
      virtual void setPosition( point );
      virtual void setRotation( rotation );
      virtual void rotate( rotation );
      virtual void rotateAbout( rotation, point );

      double getLengthX() const { return _lengthX; }
      double getLengthY() const { return _lengthY; }
      double getLengthZ() const { return _lengthZ; }

      double getSurfaceArea() const { return _areaSum; }


      const surface_rectangle* getSurfaces() const { return _surfaces; }
      unsigned int getNumSurfaces() const { return 6; }


      virtual bool contains( const point& p ) const;
      virtual bool crosses( const line& ) const;
      virtual interaction intersect( const line& ) const;

      virtual surfacemap sampleSurface() const;
      virtual line sampleEmission() const;
  };
  

////////////////////////////////////////////////////////////////////////////////////////////////////
  // Square Plane

  class square_plane : public object_base
  {
    private:
      surface_rectangle _surface;

    protected:
      void _makeSurface();

    public:
      // Lengths in x, y in the reference frame of the surface
      square_plane( material_base*, double, double );

      // Wrapping base class setters to call _makeSurfaces.
      virtual void setPosition( point );
      virtual void setRotation( rotation );
      virtual void rotate( rotation );
      virtual void rotateAbout( rotation, point );

      double getLengthX() const { return _surface.getLengthX(); }
      double getLengthY() const { return _surface.getLengthY(); }

      double getSurfaceArea() const { return _surface.getArea(); }


      virtual bool contains( const point& p ) const;
      virtual bool crosses( const line& ) const;
      virtual interaction intersect( const line& ) const;

      virtual surfacemap sampleSurface() const;
      virtual line sampleEmission() const;
  };


////////////////////////////////////////////////////////////////////////////////////////////////////
  // Circular Plane

  class circular_plane : public object_base
  {
    private:
      surface_circle _surface;

    protected:
      void _makeSurface();

    public:
      // Material and radius
      circular_plane( material_base*, double );

      // Wrapping base class setters to call _makeSurfaces.
      virtual void setPosition( point );
      virtual void setRotation( rotation );
      virtual void rotate( rotation );
      virtual void rotateAbout( rotation, point );

      double getRadius() const { return _surface.getRadius(); }

      double getSurfaceArea() const { return _surface.getArea(); }


      virtual bool contains( const point& p ) const;
      virtual bool crosses( const line& ) const;
      virtual interaction intersect( const line& ) const;

      virtual surfacemap sampleSurface() const;
      virtual line sampleEmission() const;
  };

}

#endif // S3D_PRIMITIVES_H_
