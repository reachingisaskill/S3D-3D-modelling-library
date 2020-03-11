
#include "S3D_allSurfaces.h"

namespace S3D
{

  /////////////////////////////////////////////////////////////////////////////////


  plane::plane( threeVector c, rotation r ) :
    surface( c, r  )
  {
  }

  double plane::distance( const threeVector* pt ) const
  {
    threeVector temp = *pt - getCenter();
    return ( temp * getDirection() );
  }

  double plane::distance( const line* l ) const
  {
    threeVector temp = this->getCenter() - l->getStart() ;
    return ( temp * this->getDirection() ) / ( this->getDirection() * l->getDirection() );
  }

  bool plane::crosses( const ray* l ) const
  {
    return ( inFront( &l->getStart() ) != inFront( &l->getEnd() ) );
  }

  bool plane::crosses( const line* l ) const
  {
    return ( ( l->getDirection() * this->getDirection() ) != 0.0 );
  }

  threeVector plane::intersect( const line* l ) const
  {
    return l->getStart() + ( this->distance( l ) * l->getDirection() );
  }


  /////////////////////////////////////////////////////////////////////////////////


  square_plane::square_plane( threeVector c, rotation r, double a, double b ) :
    plane( c, r ),
    _width( a ),
    _length( b ),
    _half_width( 0.5 * _width ),
    _half_length( 0.5 * _length )
  {
  }

  double square_plane::area() const 
  {
    return _width * _length;
  }

  bool square_plane::crosses( const ray* l ) const
  {
    if ( this->inFront( &l->getStart() ) == this->inFront( &l->getEnd() ) ) return false;
    threeVector inter = this->intersect( l );
    threeVector diff = inter - getCenter();

    diff = this->getRotation() / diff;

    return ( ( diff[0] <= _half_width ) && ( diff[1] <= _half_length ) ); // TODO TEST!
  }

  /////////////////////////////////////////////////////////////////////////////////


  circular_plane::circular_plane( threeVector c, rotation r, double rad ) :
    plane( c, r ),
    _radius( rad )
  {
  }

  double circular_plane::area() const
  {
    return S3D::PI * _radius * _radius;
  }

  bool circular_plane::crosses( const ray* l ) const // TODO
  {
    if ( this->inFront( &l->getStart() ) == this->inFront( &l->getEnd() ) ) return false;
    if ( l->getDirection() * this->getDirection() == 0.0 ) return false; // Parallel to Plane
    threeVector inter = this->intersect( l );
    return ( inter - this->getCenter() ).mod() <= _radius;
  }


  /////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////


  cylinder_wall::cylinder_wall( threeVector c, rotation r, double rad ) :
    surface( c, r ),
    _radius( rad ),
    _centerLine( c, r.rotateVector( unit_threeVector_z ) )
  {
  }

  double cylinder_wall::distance( const threeVector* pt ) const
  {
    double d = this->getCenterLine().distance( pt );
    return d - _radius ;
  }

  double cylinder_wall::distance( const line* l ) const // MUST BE WRONG!
  {
    threeVector sep = this->getCenterLine().separation( l );
    threeVector toCen = -1.0 * ( sep + this->getCenterLine().separation( &l->getStart() ) ); // TODO
    double chordLength = sqrt( _radius*_radius - sep.square() ); // Ok
    double distToCen = toCen.square();
    if ( distToCen != 0.0 ) distToCen /= ( toCen * l->getDirection() );
    double lineAngle = ( this->getDirection() ^ l->getDirection() ).mod();

    if ( this->inFront( &l->getStart() ) )
      return distToCen - ( chordLength / lineAngle );
    else
      return distToCen + ( chordLength / lineAngle );
  }

  bool cylinder_wall::crosses( const ray* l ) const //TODO
  {
    bool lstrt = this->inFront( &l->getStart() );
    bool lend = this->inFront( &l->getEnd() );

    if ( lstrt != lend ) return true; // Must be some boolean optimizations here!
    else if ( lstrt == true )
    {
      double dist = this->getCenterLine().distance( l );
      if ( ( fabs( dist ) > _radius ) ) return false;
      else
      {
        threeVector v1 = l->getStart() - this->getCenter(); // Do I Need to norm these vectors?
        threeVector v2 = l->getEnd() - this->getCenter(); // Are you sure this actually works?

        v1 -= ( v1 * this->getDirection() ) * this->getDirection(); // Move back to above this->getCenter()
        v2 -= ( v2 * this->getDirection() ) * this->getDirection();
        return ( ( v1 * v2 ) <= 0 );
      }
    }
    else
      return false;
  }
  
  bool cylinder_wall::crosses( const line* l ) const
  {
    if ( l->getDirection() == this->getDirection() ) return false;
    else if ( fabs( this->getCenterLine().distance( l ) ) > getRadius() ) return false;
    else return true;
  }

  threeVector cylinder_wall::intersect( const line* l ) const
  {
    return l->getStart() + ( this->distance( l ) * l->getDirection() );
  }
  

  void cylinder_wall::rotate( rotation r )
  {
    _centerLine.rotate( r );
    surface::rotate( r );
  }

  void cylinder_wall::rotateAbout( rotation r, threeVector p )
  {
    _centerLine.rotateAbout( r, p );
    surface::rotateAbout( r, p );
  }

  /////////////////////////////////////////////////////////////////////////////////


  cylinder_wall_section::cylinder_wall_section( threeVector c, rotation r, double rad, double len ) :
    cylinder_wall( c, r, rad ),
    _length( len ),
    _half_length( 0.5 * _length )
  {
  }

  bool cylinder_wall_section::crosses( const ray* l ) const // TODO
  {
    bool lstrt = this->inFront( &l->getStart() );
    bool lend = this->inFront( &l->getEnd() );

    if ( (!lstrt) && (!lend) ) return false; // Both Inside cylinder
    else if ( l->getDirection() * this->getCenterLine().getDirection() == 1.0 ) return false; // Parallel Lines
    else
    {
      double dist = this->getCenterLine().distance( l );
      if ( fabs( dist ) > this->getRadius() ) return false;
      else if ( lstrt != lend ) // One in and One out
      {
        double len = ( ( this->intersect( l ) - this->getCenter() ) * this->getCenterLine().getDirection() );
        if ( fabs( len ) <= _half_length ) return true;
        else return false;
      }
      else                       // Both Outside
      {
        threeVector v1 = l->getStart() - this->getCenter(); // Do I Need to norm these vectors?
        threeVector v2 = l->getEnd() - this->getCenter(); // Are you sure this actually works? Pretty Sure - why?

        v1 -= ( v1 * this->getDirection() ) * this->getDirection(); // Move back to above this->getCenter()
        v2 -= ( v2 * this->getDirection() ) * this->getDirection();
        if ( ( v1 * v2 ) > 0 ) return false;
        else
        {
          double len = ( ( this->intersect( l ) - this->getCenter() ) * this->getCenterLine().getDirection() );
          if ( fabs( len ) <= _half_length ) return true;
          else return false;
        }
      }
    }
  }


  /////////////////////////////////////////////////////////////////////////////////

  /*

  cutout::cutout( threeVector pos, rotation rot, unsigned int num, twoVector* points ) :
    surface( pos, rot ),
    _numPoints( num ),
    _points( new twoVector[ num ] ),
    _extent( 0.0 )
  {
    for ( unsigned long int i = 0; i < num; ++i )
    {
      double mag = pos.mod();
      if ( mag > _extent )
        _extent = mag;
      _points[ i ] = points[ i ];
    }
  }
   
  double cutout::area() const
  {
  }

  double cutout::distance( const threeVector* ) const
  {
  }

  double cutout::distance( const line* ) const
  {
  }

  bool cutout::crosses( const ray* ) const
  {
  }

  bool cutout::crosses( const line* ) const
  {
  }

  threeVector cutout::intersect( const line* ) const
  {
  }

  void cutout::rotate( rotation )
  {
  }

  void cutout::rotateAbout( rotation, threeVector )
  {
  }

  */
}

