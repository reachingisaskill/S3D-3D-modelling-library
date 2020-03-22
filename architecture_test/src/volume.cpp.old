
#include "S3D_volume.h"

#include <cmath>

namespace S3D
{

  volume_base::volume_base( double R, threeVector p, rotation r ) :
    object_3D_base( p, r ),
    _normal( r.rotateVector( unit_threeVector_z ) ),
    _outerRad( R ),
    _outerRadSq( R*R )
  {
  }

  void volume_base::rotate( rotation r )
  {
    _normal = r * _normal;
    object_base::rotate( r );
  }

  void volume_base::rotateAbout( rotation r, threeVector p )
  {
    _normal = ( r * ( _normal - p) ) + p;
    object_base::rotateAbout( r, p );
  }

//  bool volume_base::contains( threeVector vec ) const
//  {
//    return this->contains( &vec );
//  }
//
//  bool volume_base::Contains( threeVector vec ) const
//  {
//    return this->Contains( &vec );
//  }


  /*
  print_base* volume_base::_print() const
  {
    print_primitive* newShape = new print_primitive( "Sphere" );

    newShape->addParam( "radius", stdexts::stringConvert( this->outer() ) );
    newShape->setPosition( this->center() );
    //newShape.setRotation( ...

    return newShape;
  }
  */

  /////////////////////////////////////////////////////////////////////////////

  simple_shape::simple_shape() : 
    volume_base( 0.0, threeVector( 0.0 ), rotation() ),
    _numSurfaces( 0 ),
    _surfaces( 0 )
  {
  }

  simple_shape::simple_shape( unsigned int n, double R, threeVector pt, rotation rot ) : 
    volume_base( R, pt, rot ),
    _numSurfaces( n ),
    _surfaces( new surface*[_numSurfaces] )
  {
  }

  simple_shape::~simple_shape()
  {
    for ( unsigned int i = 0; i < _numSurfaces; ++i )
    {
      if ( _surfaces[i] ) delete _surfaces[i];
      _surfaces[i] = 0;
    }
    delete [] _surfaces;
  }

  bool simple_shape::contains( const threeVector* pt ) const // Includes touching the surface!
  {
    for ( unsigned int i = 0; i < this->numSurfaces(); ++i )
    {
      if ( _surfaces[i]->distance( pt ) > 0.0 ) 
      {
        // std::cout << "\nSURFACE " << i << " FAILED!\n";
        return false;
      }
    }
    return true;
  }

  bool simple_shape::Contains( const threeVector* pt ) const // Does NOT include touching the surface!
  {
    for ( unsigned int i = 0; i < this->numSurfaces(); ++i )
    {
      if ( _surfaces[i]->inFront( pt ) ) return false;
    }
    return true;
  }

  bool simple_shape::crosses( const line* l ) const
  {
    return this->_crossesSurfaces( l );
  }

  bool simple_shape::crosses( const ray* l ) const
  {
    if ( this->contains( &l->getStart() ) != this->contains( &l->getEnd() ) ) return true;
    else return this->_crossesSurfaces( l );
  }

  interaction simple_shape::intersect( const line* l ) const
  {
    return l->getStart() + this->distance( l )*l->getDirection();
    return interaction( point, l, this, ( point - this->getCenter() ).norm(), nullptr );
  }

  double simple_shape::distance( const line* l ) const
  {
    double shortestDist = HUGE_VAL;
    for ( unsigned int i = 0 ; i < this->numSurfaces(); ++i )
    {
      if ( this->surfaces()[i]->crosses( l ) )
      {
        double tmp = this->surfaces()[i]->distance( l );
        if ( tmp < shortestDist && tmp > 0.0 ) shortestDist = tmp;
      }
    }
    return shortestDist;
  }

  stdexts::fifo< double > simple_shape::distances( const line* l ) const
  {
    stdexts::fifo< double > ret;
    for ( unsigned int i = 0 ; i < this->numSurfaces(); ++i )
    {
      if ( this->surfaces()[i]->crosses( l ) )
      {
        ret << this->surfaces()[i]->distance( l );
      }
    }
    return ret;
  }

  bool simple_shape::_crossesSurfaces( const line* l ) const
  {
     for ( unsigned int i = 0 ; i < this->numSurfaces(); ++i )
     {
       if ( this->surfaces()[i]->crosses( l ) )
       {
         return true;
       }
     }
     return false;
  }

  void simple_shape::rotate( rotation r )
  {
    for ( unsigned int i = 0; i < this->numSurfaces(); ++i )
    {
      this->surfaces()[i]->rotate( r );
    }

    volume_base::rotate( r );
  }

  void simple_shape::rotateAbout( rotation r, threeVector p )
  {
    for ( unsigned int i = 0; i < this->numSurfaces(); ++i )
    {
      this->surfaces()[i]->rotateAbout( r, p );
    }

    volume_base::rotateAbout( r, p );
  }

}

