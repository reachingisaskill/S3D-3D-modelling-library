
#ifndef __S3D__CAMERA_PINHOLE_H__
#define __S3D__CAMERA_PINHOLE_H__

#include "S3D_camera_base.h"


namespace S3D
{

  class camera_pinhole : public camera_base
  {
    private :

    protected:

    public:
      // Position, Rotation, FieldofView
      camera_pinhole( threeVector, rotation, double );

      // Use the implicit copy, and move constructors
//      camera_linescan( const camera_linescan& );
//      camera_linescan& operator=( const camera_linescan& );

      // Make sure its virtual
      virtual ~camera_pinhole();

      // Override pure virtual function
      virtual void shutter();
  };

}

#endif // __S3D__CAMERA_PINHOLE_H__

