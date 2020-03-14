
#ifndef __S3D__CAMERA_TEST_H__
#define __S3D__CAMERA_TEST_H__

#include "S3D_global.h"
#include "S3D_defs.h"

#include "S3D_camera_base.h"


namespace S3D
{

  class camera_test : public camera_base
  {
    private :
      double _width; // Physical sizes
      double _height;

    protected:

    public:
      // Position, Direction, size_x, size_y
      camera_test( threeVector, threeVector, double, double );

      // Use the implicit copy, and move constructors
//      camera_linescan( const camera_linescan& );
//      camera_linescan& operator=( const camera_linescan& );

      // Make sure its virtual
      virtual ~camera_test();

      // Override pure virtual function
      virtual void shutter();
  };

}

#endif // __S3D__CAMERA_TEST_H__

