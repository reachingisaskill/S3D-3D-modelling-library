
#include "S3D_surface.h"

#include "S3D_defs.h"

#include <cmath>


namespace S3D
{

  surface::surface() :
    _position(),
    _normal(),
    _rotation()
  {
  }


  surface::surface( point p, rotation r ) :
    _position( p ),
    _normal( r.rotateVector( unit_threeVector_z ) ),
    _rotation( r )
  {
  }


  surface::~surface()
  {
  }

  
  void surface::setNormal( const threeVector& newNorm )
  {
    threeVector v = newNorm.norm();
    threeVector axis = crossProduct( _normal, v );
    double angle = vectorAngle( _normal, v );
    _rotation = rotation( axis.norm(), angle );

    _normal = v;
  }


  void surface::setRotation( const rotation& r )
  {
    _rotation = r;
    _normal = r.rotateVector( unit_threeVector_z );
  }


  bool surface::inFront( const point& p ) const
  {
    return ( this->distance( p ) > 0.0 );
  }


  double surface::distance( const point& p ) const
  {
    return ( p - this->_position ) * _normal;
  }


  double surface::distance( const line& l ) const
  {
    return ( (_position - l.getStart()) * _normal ) / ( std::fabs(_normal * l.getDirection() ) );
  }


  bool surface::crosses( const line& l ) const
  {
    if ( std::fabs( l.getDirection() * _normal ) < epsilon ) // Basically perpendicular to the normal
      return false;
    
    if ( this->distance( l ) <= 0.0 ) // Going backwards
      return false;

    return true;
  }


  point surface::intersect( const line& l ) const
  {
    return l.getStart() + this->distance( l ) * l.getDirection();
  }


  twoVector surface::planarIntersect( const line& l )
  {
    threeVector inter = this->intersect( l ) - _position;
    threeVector planarVector = _rotation / inter;
    return makeTwoVector( planarVector[0], planarVector[1] );
  }


  void surface::rotate( rotation r )
  {
    this->_normal = r * this->_normal;
    this->_rotation = r * this->_rotation;
  }


  void surface::rotateAbout( rotation r, point p )
  {
    _position = p + ( r * ( _position - p ) );
    this->rotate( r );
  }


////////////////////////////////////////////////////////////////////////////////////////////////////
  // Friend Functions

  line surfaceIntersection( const surface& surf1, const surface& surf2 )
  {
    threeVector direction = crossProduct( surf1.getNormal(), surf2.getNormal() );

    line findLine1( surf1.getPosition(), crossProduct( surf1.getNormal(), direction ) );
    line findLine2( surf2.getPosition(), crossProduct( surf2.getNormal(), direction ) );

    point start = lineIntersection( findLine1, findLine2 );

    return line( start, direction );
  }

}

