
#ifndef __S3D__CAMERA_LINE_SCAN_H__
#define __S3D__CAMERA_LINE_SCAN_H__

#include "S3D_global.h"
#include "S3D_defs.h"

#include "S3D_camera_base.h"


namespace S3D
{

  class camera_lineScan : public camera_base
  {
    private :

    protected:

    public:
      // Position, Direction, size_x, size_y
      camera_lineScan( threeVector, threeVector, double, double );

      virtual ~camera_lineScan();
  };

}

#endif // __S3D__CAMERA_LINE_SCAN_H__

