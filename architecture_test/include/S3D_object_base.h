
#ifndef __S3D__OBJECT_BASE_H__
#define __S3D__OBJECT_BASE_H__

#include "S3D_base.h"

#include "S3D_material_base.h"
#include "S3D_interaction.h"


namespace S3D
{

  // All objects that can be seen in 3D space
  class object_base : public base
  {
    private:
      material_base* _material;

      // Parameers to describe boundind ellipsoid
      double _boundA; // Max extent x ish
      double _boundB; // Max extent y ish
      double _boundC; // Max extent z ish

      threeVector _ellipse;

    protected:
      // Specify material and elliptical bound
      object_base( material_base*, double, double, double );
      // Specify material and spherical bound
      object_base( material_base*, double );

      material_base* _getMaterial() { return _material; }

      bool _bounded( const point& ) const;

    public:
      virtual ~object_base();

      const material_base* getMaterial() const { return _material; }

      // Pure virtual functions to handle intersection, containment, collision, etc.
      virtual bool contains( const point& p ) const = 0;
      virtual bool crosses( const line& ) const = 0;
      virtual double distance( const line& ) const = 0;
      virtual double distance( const point& ) const = 0;
      virtual interaction intersect( const line& ) const = 0;

  };
}

#endif // __S3D__OBJECT_BASE_H__

