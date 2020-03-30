
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

    protected:
      // Specify material and elliptical bound
      object_base( material_base* );

      material_base* _getMaterial() { return _material; }

      // Parameters: Incoming Dir, surface normal
//      threeVector _calculateSnellsLawIn( threeVector, threeVector ) const;
//      threeVector _calculateSnellsLawOut( threeVector, threeVector ) const;
//      threeVector _calculateReflection( threeVector, threeVector ) const;

    public:
      virtual ~object_base();

      const material_base* getMaterial() const { return _material; }

      // Pure virtual functions to handle intersection, containment, collision, etc.
      virtual bool contains( const point& p ) const = 0;
      // Strictly only if the distance from the start of the line is positive!!!
      virtual bool crosses( const line& ) const = 0;
      virtual interaction intersect( const line& ) const = 0;

//      virtual double distance( const line& ) const = 0;
//      virtual double distance( const point& ) const = 0;
  };
}

#endif // __S3D__OBJECT_BASE_H__

