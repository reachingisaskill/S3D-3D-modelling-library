
#include "S3D_interaction.h"

namespace S3D
{

  interaction::interaction( point p, const line* l, const object_base* s, threeVector norm, const surfaceMapping* map ) :
    _thePoint( p ),
    _theLine( l ),
    _theObject( s ),
    _surfaceNormal( norm ),
    _surfaceMapping( map ),
    _cosIncidentAngle( norm * l->getDirection() )// Both should be normalised!!!
  {
  }
}

