
#ifndef __S3D__CAMERA_H__
#define __S3D__CAMERA_H__

#include "S3D_realVector.h"
#include "S3D_rotation.h"
#include "S3D_beam.h"
#include "S3D_frame.h"
#include "S3D_raytracer.h"

#include "stdexts.h"

namespace S3D
{

  class camera_base
  {
    private:
      threeVector _position;
      rotation _rotation;
      double _fieldOfView;
      unsigned int _pixelsX;
      unsigned int _pixelsY;
      frame* _frame;
      rayTracer* _rayTracer;

    protected:
      void _setFrame( frame* f ) { _frame = f; }
      rayTracer* _getRayTracer() { return this->_rayTracer; }

    public:
      // Position, Direction, FieldOfView
      camera_base( threeVector, rotation, double=0.0 );

      // Copy constructor (does not copy the frame object!)
      camera_base( const camera_base& );

      // Move constructor (moves the frame object!)
      camera_base( camera_base&& ) noexcept;

      // Assignment operator (does not copy the frame object!)
      camera_base& operator=( const camera_base& );

      // Move assignment operator (moves the frame object!)
      camera_base& operator=( camera_base&& );

      virtual ~camera_base();

      // Set the rayTracer object - takes ownership
      void setRayTracer( rayTracer* );

      // Set the size of the frame
      virtual void setPixels( unsigned int x, unsigned int y );
      unsigned int getPixelsX() const { return _pixelsX; }
      unsigned int getPixelsY() const { return _pixelsY; }

      virtual inline const threeVector& getPosition() const { return this->_position; }
      virtual inline void setPosition( threeVector pos ) { this->_position = pos; }

      virtual inline const rotation& getRotation() const { return this->_rotation; }
      virtual inline void setRotation( rotation rot ) { this->_rotation = rot; }

      virtual inline double getFieldOfView() const { return this->_fieldOfView; }
      virtual inline void setFieldOfView( double fov ) { this->_fieldOfView = fov; }


      // Take the image
      virtual void shutter() = 0;
      // Return a pointer to frame containing the pixels and null the local pointer.
      virtual stdexts::autoPtr<frame> popFrame();
  };

}

#endif // __S3D__CAMERA_H__
