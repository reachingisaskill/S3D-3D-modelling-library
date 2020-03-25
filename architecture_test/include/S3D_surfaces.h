
#ifndef __S3D__SURFACES_H__
#define __S3D__SURFACES_H__

#include "S3D_surface.h"


namespace S3D
{

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

      // virtual point intersect( const line& ) const; // Use base class variant
  };
}

#endif // __S3D__SURFACES_H__

