
#include "S3D_line.h"

#include "S3D_defs.h"


namespace S3D
{

  line::line( point p, rotation r ) :
    _position( p ),
    _direction( r * unit_threeVector_z )
  {
  }

  line::line( point p, threeVector v ) :
    _position( p ),
    _direction( v.norm() )
  {
  }

  line::~line()
  {
  }


  rotation line::_getRotation( threeVector v )
  {
    if ( ( v ^ unit_threeVector_z ).mod() == 0.0 )
      return rotation();
    else
      return rotation( ( v ^ unit_threeVector_z ), vectorAngle( v, unit_threeVector_z ) );
  }


  threeVector line::separation( const line& l ) const
  {
    if ( ( l.getDirection() * this->getDirection() ) == 1.0 ) // Parallel
    {
      return ( this->getStart() - l.getStart() ) ^ this->getDirection();
    }
    else
    { 
      threeVector dir = ( this->getDirection() ^ l.getDirection() ).norm();
      return ( ( this->getStart() - l.getStart() ) * dir ) * dir;
    }
  }


  double line::distance( const line& l ) const
  {
    if ( l.getDirection() == this->getDirection() ) // Parallel
    {
      return ( ( this->getStart() - l.getStart() ) ^ this->getDirection() ).mod();
    }
    else
    { 
      threeVector dir = ( this->getDirection() ^ l.getDirection() ).norm();
      return ( ( this->getStart() - l.getStart() ) * dir );
    }
  }


  threeVector line::separation( const point& p ) const
  {
    threeVector sep = ( p - this->getStart() );
    threeVector dist = ( sep * this->getDirection() ) * this->getDirection();
    return sep - dist; 
  }


  double line::distance( const point& p ) const
  {
    return this->separation( p ).mod();
  }


  void line::rotate( rotation r )
  {
    _direction = r * _direction;
  }


  void line::rotateAbout( rotation r, threeVector p )
  {
    _position = ( r * ( _position - p ) ) + p;
    this->rotate( r );
  }


////////////////////////////////////////////////////////////////////////////////////////////////////
  // Friend functions


  point lineIntersection( const line& line1, const line& line2 )
  {
    threeVector delta_p = line2._position - line1._position;
    double delta_p_d1 = delta_p * line1._direction;
    double delta_p_d2 = delta_p * line2._direction;

    double d1_d2 = line1._direction * line2._direction;

    double length1 = ( delta_p_d1 - d1_d2*delta_p_d2 ) / ( 1.0 - d1_d2*d1_d2 );
    return point( line1._position + length1*line1._direction );
  }

}

