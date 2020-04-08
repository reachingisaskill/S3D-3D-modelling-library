
#include "S3D_texture.h"


namespace S3D
{

  surfacemap::surfacemap() :
    _sceneLocation(),
    _normal()
  {
  }

  surfacemap::surfacemap( point p, threeVector n ) :
    _sceneLocation( p ),
    _normal( n )
  {
  }

}

