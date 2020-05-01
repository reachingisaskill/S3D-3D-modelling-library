
#ifndef S3D_OBJECT_BASE_H_
#define S3D_OBJECT_BASE_H_

#include "S3D_base.h"
#include "S3D_material_base.h"
#include "S3D_interaction.h"
#include "S3D_texture.h"


namespace S3D
{

  // Forward declar the ray-tracer base class
  class tracer_base;


  // All objects that can be seen in 3D space
  class object_base : public base
  {
    private:
      material_base* _material;

    protected:
      // Specify material and elliptical bound
      object_base( material_base* );

      material_base* _getMaterial() { return _material; }

    public:
      virtual ~object_base();

      const material_base* getMaterial() const { return _material; }

      virtual double getSurfaceArea() const = 0;

      // Pure virtual functions to handle intersection, containment, collision, etc.
      virtual bool contains( const point& p ) const = 0;
      // Strictly only if the distance from the start of the line is positive!!!
      virtual bool crosses( const line& ) const = 0;
      virtual interaction intersect( const line& ) const = 0;

      // Returns a random point on the surface of the object,
      virtual surfacemap sampleSurface() const = 0;

      // Randomly samples the light seen at a given point
      //  ray tracer object required to perform visibility checks
      virtual line sampleEmission() const = 0;

//      virtual double distance( const line& ) const = 0;
//      virtual double distance( const point& ) const = 0;
  };
}

#endif // S3D_OBJECT_BASE_H_

