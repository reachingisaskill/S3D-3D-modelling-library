
#ifndef __S3D__CAMERA_H__
#define __S3D__CAMERA_H__

#include "S3D_vector.h"
#include "S3D_point.h"
#include "S3D_rotation.h"
#include "S3D_spectrum.h"
#include "S3D_frame.h"
#include "S3D_base.h"

#include "stdexts.h"

namespace S3D
{
  class tracer_base;

  class camera_base : public base
  {
    private:
      double _fieldOfView;
      unsigned int _pixelsX;
      unsigned int _pixelsY;
      frame* _frame;
      tracer_base* _rayTracer;

    protected:
      void _setFrame( frame* f ) { _frame = f; }
      tracer_base* _getRayTracer() { return this->_rayTracer; }

    public:
      // Ray Tracer, FieldOfView
      camera_base( tracer_base*, double=0.0 );

      virtual ~camera_base();

      // Set the rayTracer object - takes ownership
      void setRayTracer( tracer_base* );

      // Set the size of the frame
      virtual void setPixels( unsigned int x, unsigned int y );
      unsigned int getPixelsX() const { return _pixelsX; }
      unsigned int getPixelsY() const { return _pixelsY; }


      virtual inline double getFieldOfView() const { return this->_fieldOfView; }
      virtual inline void setFieldOfView( double fov ) { this->_fieldOfView = fov; }


      // Take the image
      virtual void shutter() = 0;
      // Return a pointer to frame containing the pixels and null the local pointer.
      virtual stdexts::autoPtr<frame> popFrame();
  };

}

#endif // __S3D__CAMERA_H__
