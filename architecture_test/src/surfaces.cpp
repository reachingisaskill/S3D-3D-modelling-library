
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
    threeVector inter = this->intersect( l ) - l.getStart();
    if ( inter * l.getDirection() < epsilon ) // Going backwards
      return false;

    threeVector ref_inter = this->getRotation() / ( this->intersect( l ) - this->getPosition() );

//    if ( std::fabs(2.0*ref_inter[0]) > _widthX )
    if ( ( std::fabs(2.0*ref_inter[0]) - _widthX ) > epsilon )
      return false;
//    if ( std::fabs(2.0*ref_inter[1]) > _widthY )
    if ( ( std::fabs(2.0*ref_inter[1]) - _widthY ) > epsilon )
      return false;

    return true;
  }

}

