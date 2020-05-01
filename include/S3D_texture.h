
#ifndef S3D_TEXTURE_BASE_H_
#define S3D_TEXTURE_BASE_H_

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
    threeVector _normal;

    public:
      surfacemap();
      surfacemap( point, threeVector );

      point getPosition() const { return _sceneLocation; }
      threeVector getNormal() const { return _normal; }
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

#endif // S3D_TEXTURE_BASE_H_

