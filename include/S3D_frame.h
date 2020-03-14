
#ifndef __S3D__FRAME_H__
#define __S3D__FRAME_H__

#include "S3D_global.h"
#include "S3D_defs.h"

namespace S3D
{


  // All the detail that can be carried by a beam of light.
  struct beam
  {
    double red;
    double green;
    double blue;
    unsigned int numReflections;

    beam();
  };


  // 2D image of pixels a single frame.
  class frame
  {
    private:
      unsigned int _pixelsX;
      unsigned int _pixelsY;
      beam* _image;

      unsigned int _pixelNum( unsigned int x, unsigned int y ) const;

    protected:

    public:
      // Num pixels x & y
      frame( unsigned int, unsigned int );

      frame( const frame& );

      virtual frame& operator=( const frame& );

      virtual ~frame();

      unsigned int numPixelsX() const { return this->_pixelsX; }
      unsigned int numPixelsY() const { return this->_pixelsY; }

      // Return a reference to a specific pixel
      beam& pixel( unsigned int x, unsigned int y ) { return _image[ _pixelNum( x, y ) ]; }
      const beam& pixel( unsigned int x, unsigned int y ) const { return _image[ _pixelNum( x, y ) ]; }

      // Save the frame to a bmp image
      void dump( std::string ) const;
  };

}

#endif // __S3D__FRAME_H__

