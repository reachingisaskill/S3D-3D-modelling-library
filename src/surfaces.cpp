#define LOGTASTIC_DEBUG_OFF

#include "S3D_surfaces.h"

#include "S3D_defs.h"
#include "S3D_random.h"

#include "logtastic.h"

#include <cmath>
#include <cassert>


namespace S3D
{

// Rectangular Surface 

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
    DEBUG_STREAM << "surface_rectangle::crosses() : Line = " << l.getStart().getPosition() << " -- " << l.getDirection() << ".   Normal = " << getNormal();
    if ( std::fabs( l.getDirection() * getNormal() ) < epsilon ) // Basically perpendicular to the normal
    {
      return false;
    }

    threeVector inter = this->intersect( l ) - l.getStart();
    if ( inter * l.getDirection() < epsilon ) // Going backwards
      return false;

    threeVector ref_inter = this->getRotation() / ( this->intersect( l ) - this->getPosition() );

    if ( ( std::fabs(2.0*ref_inter[0]) - _widthX ) > epsilon )
      return false;
    if ( ( std::fabs(2.0*ref_inter[1]) - _widthY ) > epsilon )
      return false;

    return true;
  }


  point surface_rectangle::sampleSurface() const
  {
    twoVector rand_pos = random::uniformRectangularPlane( _widthX, _widthY );
    threeVector pos = this->getRotation().rotateVector( makeThreeVector( rand_pos[0], rand_pos[1], 0.0 ) );
    return point( this->getPosition() + pos );
  }


////////////////////////////////////////////////////////////////////////////////////////////////////
  // Circular Surface 


  surface_circle::surface_circle() :
    surface(),
    _radius(),
    _radiusSq()
  {
  }


  surface_circle::surface_circle( double r ) :
    surface(),
    _radius( r ),
    _radiusSq( r*r )
  {
  }


  bool surface_circle::crosses( const line& l ) const
  {
    threeVector inter = this->intersect( l ) - l.getStart();
    if ( inter * l.getDirection() < epsilon ) // Going backwards
      return false;

    threeVector ref_inter = this->getRotation() / ( this->intersect( l ) - this->getPosition() );
    double dist = ref_inter.square();

    if ( ( dist - _radiusSq ) > epsilon )
      return false;

    return true;
  }


  point surface_circle::sampleSurface() const
  {
    twoVector rand_pos = random::uniformCircularPlane( _radius );
    threeVector pos = this->getRotation().rotateVector( makeThreeVector( rand_pos[0], rand_pos[1], 0.0 ) );
    return point( this->getPosition() + pos );
  }

}

