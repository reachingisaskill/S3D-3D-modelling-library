
#ifndef __S3D__PRIMITIVES_H__
#define __S3D__PRIMITIVES_H__

#include "S3D_point.h"
#include "S3D_line.h"
#include "S3D_rotation.h"
#include "S3D_object_base.h"
#include "S3D_surfaces.h"
#include "S3D_interaction.h"
#include "S3D_material_base.h"

namespace S3D
{

  class sphere : public object_base
  {
    private:
      double _radius;
    protected:
    public:
      sphere( material_base*, double );

      double getRadius() const { return _radius; }

      virtual bool contains( const point& p ) const;
      virtual bool crosses( const line& ) const;
      virtual interaction intersect( const line& ) const;

      virtual double distance( const line& ) const;
      virtual double distance( const point& ) const;
  };


  class box : public object_base
  {
    private:
      double _lengthX;
      double _lengthY;
      double _lengthZ;

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


      virtual bool contains( const point& p ) const;
      virtual bool crosses( const line& ) const;
      virtual interaction intersect( const line& ) const;

  };

}

#endif // __S3D__PRIMITIVES_H__
