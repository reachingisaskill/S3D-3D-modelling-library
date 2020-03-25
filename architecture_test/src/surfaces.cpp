
#include "S3D_surfaces.h"

#include "S3D_defs.h"

#include "logtastic.h"
#include <cmath>
#include <cassert>


namespace S3D
{

  surface_rectangle::surface_rectangle() :
    surface(),
    _widthX( 0.0 ),
    _widthY( 0.0 )
  {
  }


  surface_rectangle::surface_rectangle( double x, double y ) :
    surface(),
    _widthX( x ),
    _widthY( y )
  {
  }


  bool surface_rectangle::crosses( const line& l ) const
  {
    if ( std::fabs( l.getDirection() * this->getNormal() ) < epsilon ) // Basically perpendicular to the normal
      return false;

    threeVector inter = this->intersect( l ) - l.getStart();
    INFO_STREAM << "CROSSES INTERSECT: " << inter;
    if ( inter * l.getDirection() <= 0.0 ) // Going backwards
      return false;

    threeVector ref_inter = this->getRotation() / ( this->intersect( l ) - this->getPosition() );

    if ( std::fabs(2.0*ref_inter[0]) > _widthX )
      return false;
    if ( std::fabs(2.0*ref_inter[1]) > _widthY )
      return false;

    INFO_LOG( "INTERSECTS!!!" );
    return true;
  }

}

