
#ifndef __SURFACE_H__
#define __SURFACE_H__


#include "S3D_global.h"
#include "S3D_defs.h"

#include "S3D_allLines.h"


namespace S3D
{

  class surface : public object_base
  {
    private:
      threeVector _normal;

    protected:
      surface( threeVector, rotation );

    public:
      virtual ~surface();

      virtual const threeVector& getDirection() const { return _normal; }

      virtual inline bool inFront( threeVector vec ) const { return this->inFront( &vec ); }
      virtual inline bool InFront( threeVector vec ) const { return this->InFront( &vec ); };
      virtual bool inFront( const threeVector* ) const;
      virtual bool InFront( const threeVector* ) const;

      virtual double area() const = 0;

      virtual double distance( const threeVector* ) const = 0;
      virtual double distance( const line* ) const = 0;
      virtual bool crosses( const ray* ) const = 0;
      virtual bool crosses( const line* ) const = 0;
      virtual threeVector intersect( const line* ) const = 0;

      virtual void rotate( rotation );
      virtual void rotateAbout( rotation, threeVector );
  };








#ifdef S3D_TEST_FUNCTIONALITY

  class test_surface : public surface
  {
    private:

    public:
      test_surface( threeVector p, rotation r ) : surface( p, r ) {}

      virtual double area() const { return 0.0; }
      virtual double distance( const threeVector* ) const { return 0.0; }
      virtual double distance( const line* ) const { return 0.0; }
      virtual bool crosses( const ray* ) const { return false; }
      virtual bool crosses( const line* ) const { return false; }
      virtual threeVector intersect( const line* ) const { return threeVector( 0.0 ); }
  };

#endif

}

#endif // __SURFACE_H__

