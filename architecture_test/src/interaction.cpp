
#include "S3D_interaction.h"

#include "S3D_defs.h"


namespace S3D
{

  interaction::interaction() :
    _thePoint(),
    _theLine( nullptr ),
    _theObject( nullptr ),
    _surfaceNormal( unit_threeVector_z ),
    _distance( 1.0E20 )
  {
  }

  interaction::interaction( point p, const line* l, const object_base* s, threeVector norm ) :
    _thePoint( p ),
    _theLine( l ),
    _theObject( s ),
    _surfaceNormal( norm ),
    _distance( 0.0 )
  {
    _distance = ( p - l->getStart()).mod();
  }
}

