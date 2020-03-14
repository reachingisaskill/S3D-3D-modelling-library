
#ifndef __S3D__CAMERA_H__
#define __S3D__CAMERA_H__

#include "S3D_global.h"
#include "S3D_defs.h"

#include "S3D_frame.h"

namespace S3D
{

  class camera_base
  {
    private:
      threeVector _position;
      threeVector _direction;
      double _fieldOfView;
      unsigned int _pixelsX;
      unsigned int _pixelsY;
      frame* _frame;

    protected:

    public:
      // Position, Direction, FieldOfView
      camera_base( threeVector, threeVector, double=0.0 );

      // Copy constructor (does not copy the frame object!)
      camera_base( const camera_base& );

      // Move constructor (moves the frame object!)
      camera_base( camera_base&& ) noexcept;

      // Assignment operator (does not copy the frame object!)
      camera_base& operator=( const camera_base& );

      // Move assignment operator (moves the frame object!)
      camera_base& operator=( camera_base&& );

      virtual ~camera_base();


      // Set the size of the frame
      virtual void setPixels( double x, double y );

      virtual inline const threeVector& getPosition() const { return this->_position; }
      virtual inline void setPosition( threeVector pos ) { this->_position = pos; }

      virtual inline const threeVector& getDirection() const { return this->_direction; }
      virtual inline void setDirection( threeVector dir ) { this->_direction = dir; }

      virtual inline double getFieldOfView() const { return this->_fieldOfView; }
      virtual inline void setFieldOfView( double fov ) { this->_fieldOfView = fov; }


      // Take the image
      virtual void shutter() = 0;
      // Return a pointer to frame containing the pixels and null the local pointer.
      virtual stdexts::autoPtr<frame> popFrame();
  };

}

#endif // __S3D__CAMERA_H__
