
#include "S3D_intersection.h"

namespace S3D
{

  intersection_base::intersection_base( volume_base* v1, volume_base* v2, double rad, threeVector pos, rotation rot ) :
    volume_base( rad, pos, rot ),
    _shape1( v1 ),
    _shape2( v2 )
  {
  }

  intersection_base::~intersection_base()
  {
  }

  void intersection_base::rotate( rotation rot )
  {
    _shape2->rotateAbout( rot, this->getCenter() );
  }

  void intersection_base::rotateAbout( rotation rot, threeVector p )
  {
    _shape1->rotateAbout( rot, p );
    _shape2->rotateAbout( rot, p );
  }

}

