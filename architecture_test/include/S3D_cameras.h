
#ifndef __S3D__CAMERAS_H__
#define __S3D__CAMERAS_H__

#include "S3D_camera_base.h"


namespace S3D
{
////////////////////////////////////////////////////////////////////////////////////////////////////
  // Line Scan Camera

  class camera_lineScan : public camera_base
  {
    private :
      double _width; // Physical sizes
      double _height;

    protected:

    public:
      // Position, Rotation, size_x, size_y
      camera_lineScan( point, rotation, double, double );

      // Use the implicit copy, and move constructors
//      camera_linescan( const camera_linescan& );
//      camera_linescan& operator=( const camera_linescan& );

      // Make sure its virtual
      virtual ~camera_lineScan();

      // Override pure virtual function
      virtual void shutter();
  };


////////////////////////////////////////////////////////////////////////////////////////////////////
  // Perfect Pin Hole Camera

  class camera_pinhole : public camera_base
  {
    private :

    protected:

    public:
      // Position, Rotation, FieldofView
      camera_pinhole( point, rotation, double );

      // Make sure its virtual
      virtual ~camera_pinhole();

      // Override pure virtual function
      virtual void shutter();
  };
}

#endif // __S3D__CAMERAS_H__

