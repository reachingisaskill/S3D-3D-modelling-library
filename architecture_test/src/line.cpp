
#include "S3D_line.h"


namespace S3D
{

  line::line( point p, rotation r ) :
    _position( p ),
    _direction( r * unit_threeVector_z )
  {
  }

  line::line( threeVector p, threeVector v ) :
    _position( p, _getRotation( v ) ),
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
      return rotation( ( v ^ unit_threeVector_z ), angle( v, unit_threeVector_z ) );
  }


  threeVector line::separation( const line& l ) const
  {
    if ( ( l->getDirection() * this->getDirection() ) == 1.0 ) // Parallel
    {
      return ( this->getStart() - l->getStart() ) ^ this->getDirection();
    }
    else
    { 
      threeVector dir = ( this->getDirection() ^ l->getDirection() ).norm();
      return ( ( this->getStart() - l->getStart() ) * dir ) * dir;
    }
  }


  double line::distance( const line& p ) const
  {
    if ( l->getDirection() == this->getDirection() ) // Parallel
    {
      return ( ( this->getStart() - l->getStart() ) ^ this->getDirection() ).mod();
    }
    else
    { 
      threeVector dir = ( this->getDirection() ^ l->getDirection() ).norm();
      return ( ( this->getStart() - l->getStart() ) * dir );
    }
  }


  threeVector line::separation( const point& p ) const
  {
    threeVector sep = ( p->getPosition() - this->getStart() );
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
    _direction = r * _direction;
  }
}

