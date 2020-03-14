
#include "S3D_camera_linescan.h"

namespace S3D
{

  camera_lineScan::camera_lineScan( threeVector p, threeVector d, double x, double y ) :
    camera_base( p, d, 0.0 ),
    _width( x ),
    _height( y )
  {
  }

  camera_lineScan::~camera_lineScan()
  {
  }

  void camera_lineScan::shutter()
  {
  }

}

