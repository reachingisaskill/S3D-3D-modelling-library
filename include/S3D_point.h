
#ifndef __S3D__POINT_H__
#define __S3D__POINT_H__


#include "S3D_realVector.h"
#include "S3D_rotation.h"

#include "S3D_base.h"


namespace S3D
{

  class rotation;

  class point : public object_base
  {
    private:

    public:
      point( threeVector = threeVector( 0.0 ) );
      explicit point( double );
      point( double, double, double );
      virtual ~point();

      // virtual void rotate( rotation );
      // virtual void rotateAbout( rotation, threeVector );

      point& operator= ( threeVector );
  };

}

#endif // __S3D__POINT_H__

