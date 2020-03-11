
#include "S3D_line.h"


namespace S3D
{

  line::line( threeVector p, rotation r ) :
    object_base( p, r ),
    _direction( r * unit_threeVector_z )
  {
  }

  line::line( threeVector p, threeVector v ) :
    object_base( p, _getRotation( v ) ),
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


  threeVector line::separation( const line* l ) const
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

  double line::distance( const line* l ) const
  {
    // if ( ( l.getDirection() * this->getDirection() ) == 1.0 ) // Parallel
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

  threeVector line::separation( const threeVector* v ) const
  {
    threeVector sep = ( *v - this->getStart() );
    threeVector dist = ( sep * this->getDirection() ) * this->getDirection();
    return sep - dist; 
  }

  double line::distance( const threeVector* v ) const
  {
    return this->separation( v ).mod();
  }

  void line::rotate( rotation r )
  {
    _direction = r * _direction;
    object_base::rotate( r );
  }

  void line::rotateAbout( rotation r, threeVector p )
  {
    _direction = r * _direction;
    object_base::rotateAbout( r, p );
  }

  /*
  print_base* line::_print() const
  {
    print_lineSet* newShape = new print_lineSet( "IndexedLineSet" );
    newShape->addPoint( this->getStart() );
    newShape->addPoint( this->getStart() + ( this->getDirection() * 1000.0 ) );

    return newShape;
  }
  */

}

