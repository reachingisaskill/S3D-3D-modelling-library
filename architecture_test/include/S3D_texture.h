
#ifndef __S3D__TEXTURE_BASE_H__
#define __S3D__TEXTURE_BASE_H__

#include "S3D_vector.h"
#include "S3D_point.h"


namespace S3D
{

  /* 
     * Class that provides the mapping info between a position on the surface of an
     * object and the specific pixel in the texture map.
   */
  class surfacemap
  {
    point _sceneLocation;
    public:
      surfacemap();
      surfacemap( point );

      point getPosition() const { return _sceneLocation; }
  };



  /*
     * Class that stores the texture data and provides look up info based on a
     * texture mapping object - the unique surface location for an object
   */
  class texture
  {
    private:
    protected:
    public:
  };
}

#endif // __S3D__TEXTURE_BASE_H__

