
#include "S3D_allShapes.h"


namespace S3D
{
///////////////////////////////////////////////////////////////////////////////
// SPHERE
///////////////////////////////////////////////////////////////////////////////

  sphere::sphere() :
    simple_shape(),
    _radius( 0.0 )
  {
  }

  sphere::sphere( double rad, threeVector pos ) :
    simple_shape( 0, rad, pos, rotation() ),
    _radius( rad )
  {
  }

  sphere::~sphere()
  {
  }

  bool sphere::contains( const threeVector* p ) const
  {
    if ( ( this->getCenter() - *p ).mod() <= this->getRadius() )
      return true;
    else
      return false;
  }

  bool sphere::Contains( const threeVector* p ) const
  {
    if ( ( this->getCenter() - *p ).mod() < this->getRadius() )
      return true;
    else
      return false;
  }

  bool sphere::crosses( const line* l ) const
  {
    if ( this->contains( &l->getStart() ) )
    {
      return true;
    }
    else
    {
      return ( l->distance( &this->getCenter() ) <= this->getRadius() );
    }
  }

  bool sphere::crosses( const ray* l ) const
  {
    bool contstart = this->contains( &l->getStart() );
    bool contend = this->contains( &l->getEnd() );
    if ( contstart != contend ) // One In One Out
    { 
      return true;
    }
    else if ( contstart == true ) // Both Inside
    {
      return false;
    }
    else
    {
      return ( l->distance( &this->getCenter() ) <= this->getRadius() );
    }
  }

  double sphere::distance( const line* l ) const
  {
    return ( l->distance( &this->getCenter() ) - this->getRadius() );
  }

  stdexts::fifo< double > sphere::distances( const line* l ) const
  {
    stdexts::fifo< double > dists;
    dists << this->distance( l );
    return dists;
  }

  threeVector sphere::intersect( const line* l ) const // TODO Check this one;
  {
    threeVector sep = l->separation( &this->getCenter() );
    double dist = sep.mod();
    return this->getCenter() + ( ( this->getRadius() / dist ) * sep );
  }

///////////////////////////////////////////////////////////////////////////////
// BOX
///////////////////////////////////////////////////////////////////////////////

  box::box() :
    simple_shape(),
    _halfWidth_x( 0 ),
    _halfWidth_y( 0 ),
    _halfWidth_z( 0 ),
    _width_x( 0 ),
    _width_y( 0 ),
    _width_z( 0 )
  {
  }

  box::box( double x, double y, double z, threeVector pt, rotation rot ) :
    simple_shape( 6, (x*x + y*y + z*z), pt, rot ),
    _halfWidth_x( 0.5*x ),
    _halfWidth_y( 0.5*y ),
    _halfWidth_z( 0.5*z ),
    _width_x( x ),
    _width_y( y ),
    _width_z( z )
  {
    this->makeSurfaces();
  }


  box::~box()
  {
  }

  void box::makeSurfaces()
  {
    threeVector tmp = getCenter();
    tmp[0] += _halfWidth_x;
    _surfaces[0] = new square_plane( tmp, rotation( unit_threeVector_y, 0.5*PI ), _width_y, _width_z );
    tmp[0] -= _width_x;
    _surfaces[1] = new square_plane( tmp, rotation( unit_threeVector_y, -0.5*PI ), _width_y, _width_z );

    tmp = getCenter();
    tmp[1] += _halfWidth_y;
    _surfaces[2] = new square_plane( tmp, rotation( unit_threeVector_x, -0.5*PI ), _width_x, _width_z );
    tmp[1] -= _width_y;
    _surfaces[3] = new square_plane( tmp, rotation( unit_threeVector_x, 0.5*PI ), _width_x, _width_z );

    tmp = getCenter();
    tmp[2] += _halfWidth_z;
    _surfaces[4] = new square_plane( tmp, rotation(), _width_y, _width_x );
    tmp[2] -= _width_z;
    _surfaces[5] = new square_plane( tmp, rotation( unit_threeVector_y, PI ), _width_y, _width_x );

    for ( unsigned int i = 0; i < 6; ++i )
      _surfaces[i]->rotateAbout( this->getRotation(), this->getCenter() );
  }

  bool box::contains( const threeVector* pt ) const
  {
    for ( unsigned int i = 0; i < this->numSurfaces(); ++i )
    {
      if ( _surfaces[i]->inFront( pt ) ) return false;
    }
    return true;
  }

  bool box::Contains( const threeVector* pt ) const
  {
    for ( unsigned int i = 0; i < this->numSurfaces(); ++i )
    {
      if ( _surfaces[i]->InFront( pt ) ) return false;
    }
    return true;
  }

  bool box::crosses( const line* l ) const
  {
    if ( this->contains( &l->getStart() ) ) // Both Inside
    {
      return true;
    }
    else
    {
      return this->_crossesSurfaces( l );
    }
  }

  bool box::crosses( const ray* l ) const
  {
    bool contstart = this->contains( &l->getStart() );
    bool contend = this->contains( &l->getEnd() );
    if ( contstart != contend ) // One In One Out
    { 
      return true;
    }
    else if ( contstart == true ) // Both Inside
    {
      return false;
    }
    else
    {
      return this->_crossesSurfaces( l );
    }
  }


  /*
  print_base* box::_print() const
  {
    print_primitive* newShape = new print_primitive( "Box" );
    std::string size = stdexts::stringConvert( _width_x ) + ' ' +
                       stdexts::stringConvert( _width_y ) + ' ' +
                       stdexts::stringConvert( _width_z );
    newShape->addParam( "size",  size );
    newShape->setPosition( this->getCenter() );
    newShape->setRotation( 1.0, 0.0, 0.0, 1.570796 );

    return newShape;
  }
  */

///////////////////////////////////////////////////////////////////////////////
// CYLINDER
///////////////////////////////////////////////////////////////////////////////


  cylinder::cylinder() :
    simple_shape(),
    _centerLine( new line( threeVector( 0.0 ), threeVector( 0.0 ) ) ),
    _radius( 0.0 ),
    _height( 0.0 ),
    _halfHeight( 0.0 )
  {
  }

  cylinder::cylinder( double rad, double height, threeVector pt, rotation rot ) :
    simple_shape( 3, (rad*rad + height*height), pt, rot ),
    _centerLine( new line( pt, rot.rotateVector( unit_threeVector_z ) ) ),
    _radius( rad ),
    _height( height ),
    _halfHeight( 0.5 * _height )
  {
    this->makeSurfaces();
  }

  cylinder::~cylinder()
  {
    delete _centerLine;
  }

  void cylinder::makeSurfaces()
  {
    threeVector tmp = this->getCenter();

    _surfaces[1] = new cylinder_wall_section( tmp, rotation(), _radius, _height );

    tmp[2] += _halfHeight;
    _surfaces[0] = new circular_plane( tmp, rotation(), _radius );

    tmp[2] -= _height;
    _surfaces[2] = new circular_plane( tmp, rotation( unit_threeVector_x, S3D::PI ), _radius );

    for ( unsigned int i = 0; i < 3; ++i )
      _surfaces[i]->rotateAbout( this->getRotation(), this->getCenter() );
  }

  bool cylinder::contains( const threeVector* pt ) const
  {
    for ( unsigned int i = 0; i < this->numSurfaces(); ++i )
    {
      if ( _surfaces[i]->inFront( pt ) ) return false;
    }
    return true;
  }

  bool cylinder::Contains( const threeVector* pt ) const
  {
    for ( unsigned int i = 0; i < this->numSurfaces(); ++i )
    {
      if ( _surfaces[i]->InFront( pt ) ) return false;
    }
    return true;
  }

  bool cylinder::crosses( const line* l ) const
  {
    if ( this->contains( &l->getStart() ) ) // Both Inside
    {
      return true;
    }
    else
    {
      return this->_crossesSurfaces( l );
    }
  }

  bool cylinder::crosses( const ray* l ) const
  {
    bool contstart = this->contains( &l->getStart() );
    bool contend = this->contains( &l->getEnd() );
    if ( contstart != contend ) // One In One Out
    { 
      return true;
    }
    else if ( contstart == true ) // Both Inside
    {
      return false;
    }
    else
    {
      return this->_crossesSurfaces( l );
    }
  }

  void cylinder::rotate( rotation r )
  {
    this->_centerLine->rotate( r );
    simple_shape::rotate( r );
  }

  void cylinder::rotateAbout( rotation r, threeVector p )
  {
    this->_centerLine->rotateAbout( r, p );
    simple_shape::rotateAbout( r, p );
  }



//  print_base* cylinder::_print() const
//  {
//    print_primitive* newShape = new print_primitive( "Cylinder" );
//    
//    newShape->addParam( "radius",  stdexts::stringConvert( radius() ) );
//    newShape->addParam( "height",  stdexts::stringConvert( height() ) );
//    newShape->addParam( "side",  "TRUE" );
//    newShape->addParam( "bottom",  "TRUE" );
//    newShape->addParam( "top",  "TRUE" );
//    newShape->setPosition( this->getCenter() );
//    newShape->setRotation( 1.0, 0.0, 0.0, 1.570796 );
//
//    return newShape;
//  }


}

