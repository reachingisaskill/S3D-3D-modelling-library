
#include "S3D_allIntersections.h"

#include <cmath>

namespace S3D
{

////////////////////////////////////////////////////////////////////////////////////////////////////

  // Creation operators


//  subtraction operator-( volume_base* s1, volume_base* s2 )
//  {
//    return new subtraction( s1, s2 );
//  }
//
//
//  addition operator+( volume_base* s1, volume_base* s2 )
//  {
//    return new addition( s1, s2 );
//  }
//
//
//  intersection operator*( volume_base* s1, volume_base* s2 )
//  {
//    return new intersection( s1, s2 );
//  }
//
//
//  exclusion operator^( volume_base* s1, volume_base* s2 )
//  {
//    return new exclusion( s1, s2 );
//  }
//

////////////////////////////////////////////////////////////////////////////////////////////////////

  addition::addition( volume_base* v1, volume_base* v2, rotation rot ) :
    intersection_base( v1, v2, v1->getOuter() + v2->getOuter(),  0.5 * ( v1->getCenter() + v2->getCenter() ), rot )
  {
  }

  addition::~addition()
  {
  }

  bool addition::contains( const threeVector* pt ) const
  {
    return ( getShape1()->contains( pt ) ) || ( getShape2()->contains( pt ) );
  }

  bool addition::Contains( const threeVector* pt ) const
  {
    return ( getShape1()->Contains( pt ) ) || ( getShape2()->Contains( pt ) );
  }

  bool addition::crosses( const line* l ) const
  {
    return ( ( ( getShape1()->crosses( l ) ) && ( ! getShape2()->Contains( getShape1()->intersect( l ) ) ) ) || ( ( getShape2()->crosses( l ) ) && ( ! getShape1()->Contains( getShape2()->intersect( l ) ) ) ) );
  }

  bool addition::crosses( const ray* l ) const
  {
    return ( ( ( getShape1()->crosses( l ) ) && ( ! getShape2()->Contains( getShape1()->intersect( l ) ) ) ) || ( ( getShape2()->crosses( l ) ) && ( ! getShape1()->Contains( getShape2()->intersect( l ) ) ) ) );
  }

  double addition::distance( const line* l ) const
  {
    double shortestDist = HUGE_VAL;

    stdexts::fifo< double > dists1 = getShape1()->distances( l );
    while ( dists1 )
    {
      double tmp = *dists1;
      if ( ! getShape2()->Contains( l->getStart() + tmp*l->getDirection() ) )
        if ( ( tmp >= 0.0 ) && tmp < shortestDist ) shortestDist = tmp;
    }

    stdexts::fifo< double > dists2 = getShape2()->distances( l );
    while ( dists2 )
    {
      double tmp = *dists2;
      if ( ! getShape1()->Contains( l->getStart() + tmp*l->getDirection() ) )
        if ( ( tmp >= 0.0 ) && tmp < shortestDist ) shortestDist = tmp;
    }
    return shortestDist;
  }

  stdexts::fifo< double > addition::distances( const line* l ) const
  {
    return ( getShape1()->distances( l ) << getShape2()->distances( l ) );
  }

  threeVector addition::intersect( const line* l ) const
  {
    return l->getStart() + this->distance( l )*l->getDirection();
  }


////////////////////////////////////////////////////////////////////////////////////////////////////

  subtraction::subtraction( volume_base* v1, volume_base* v2, rotation rot ) :
    intersection_base( v1, v2, v1->getOuter() + v2->getOuter(),  v1->getCenter(), rot )
  {
  }

  subtraction::~subtraction()
  {
  }

  bool subtraction::contains( const threeVector* pt ) const
  {
    return ( ( getShape1()->contains( pt ) ) && ( !getShape2()->Contains( pt ) ) );
  }

  bool subtraction::Contains( const threeVector* pt ) const
  {
    return ( ( getShape1()->Contains( pt ) ) && ( !getShape2()->contains( pt ) ) );
  }

  bool subtraction::crosses( const line* l ) const
  {
    return ( ( ( getShape1()->crosses( l ) ) && ( !getShape2()->Contains( getShape1()->intersect( l ) ) ) ) || ( getShape2()->crosses( l ) && ( getShape1()->contains( getShape2()->intersect( l ) ) ) ) );
  }

  bool subtraction::crosses( const ray* l ) const
  {
    return ( ( ( getShape1()->crosses( l ) ) && ( !getShape2()->Contains( getShape1()->intersect( l ) ) ) ) || ( getShape2()->crosses( l ) && ( getShape1()->contains( getShape2()->intersect( l ) ) ) ) );
  }

  double subtraction::distance( const line* l ) const
  {
    double shortestDist = HUGE_VAL;

    stdexts::fifo< double > dists1 = getShape1()->distances( l );
    while ( dists1 )
    {
      double tmp = *dists1;
      if ( ! getShape2()->Contains( l->getStart() + tmp*l->getDirection() ) )
        if ( ( tmp >= 0.0 ) && tmp < shortestDist ) shortestDist = tmp;
    }

    stdexts::fifo< double > dists2 = getShape2()->distances( l );
    while ( dists2 )
    {
      double tmp = *dists2;
      if ( getShape1()->contains( l->getStart() + tmp*l->getDirection() ) )
        if ( ( tmp >= 0.0 ) && tmp < shortestDist ) shortestDist = tmp;
    }
    return shortestDist;
  }

  stdexts::fifo< double > subtraction::distances( const line* l ) const
  {
    return ( getShape1()->distances( l ) << getShape2()->distances( l ) );
  }

  threeVector subtraction::intersect( const line* l ) const
  {
    return l->getStart() + this->distance( l )*l->getDirection();
  }

////////////////////////////////////////////////////////////////////////////////////////////////////

  intersection::intersection( volume_base* v1, volume_base* v2, rotation rot ) :
    intersection_base( v1, v2, v1->getOuter() + v2->getOuter(),  0.5 * ( v1->getCenter() + v2->getCenter() ), rot )
  {
  }

  intersection::~intersection()
  {
  }

  bool intersection::contains( const threeVector* pt ) const
  {
    return ( getShape1()->contains( pt ) ) && ( getShape2()->contains( pt ) );
  }

  bool intersection::Contains( const threeVector* pt ) const
  {
    return ( getShape1()->Contains( pt ) ) && ( getShape2()->Contains( pt ) );
  }

  bool intersection::crosses( const line* l ) const
  {
    return ( ( ( getShape1()->crosses( l ) ) && ( getShape2()->contains( getShape1()->intersect( l ) ) ) ) || ( ( getShape2()->crosses( l ) ) && ( getShape1()->contains( getShape2()->intersect( l ) ) ) ) );
  }

  bool intersection::crosses( const ray* l ) const
  {
    return ( ( ( getShape1()->crosses( l ) ) && ( getShape2()->contains( getShape1()->intersect( l ) ) ) ) || ( ( getShape2()->crosses( l ) ) && ( getShape1()->contains( getShape2()->intersect( l ) ) ) ) );
  }

  double intersection::distance( const line* l ) const
  {
    double shortestDist = HUGE_VAL;

    stdexts::fifo< double > dists1 = getShape1()->distances( l );
    while ( dists1 )
    {
      double tmp = *dists1;
      if ( getShape2()->contains( l->getStart() + tmp*l->getDirection() ) )
        if ( ( tmp >= 0.0 ) && tmp < shortestDist ) shortestDist = tmp;
    }

    stdexts::fifo< double > dists2 = getShape2()->distances( l );
    while ( dists2 )
    {
      double tmp = *dists2;
      if ( getShape1()->contains( l->getStart() + tmp*l->getDirection() ) )
        if ( ( tmp >= 0.0 ) && tmp < shortestDist ) shortestDist = tmp;
    }
    return shortestDist;
  }

  stdexts::fifo< double > intersection::distances( const line* l ) const
  {
    return ( getShape1()->distances( l ) << getShape2()->distances( l ) );
  }

  threeVector intersection::intersect( const line* l ) const
  {
    return l->getStart() + this->distance( l )*l->getDirection();
  }
 
////////////////////////////////////////////////////////////////////////////////////////////////////

  exclusion::exclusion( volume_base* v1, volume_base* v2, rotation rot ) :
    intersection_base( v1, v2, v1->getOuter() + v2->getOuter(),  0.5 * ( v1->getCenter() + v2->getCenter() ), rot )
  {
  }

  exclusion::~exclusion()
  {
  }

  bool exclusion::contains( const threeVector* pt ) const
  {
    return ( getShape1()->contains( pt ) ) ^ ( getShape2()->contains( pt ) );
  }

  bool exclusion::Contains( const threeVector* pt ) const
  {
    return ( getShape1()->Contains( pt ) ) ^ ( getShape2()->Contains( pt ) );
  }

  bool exclusion::crosses( const line* l ) const
  {
    return ( ( getShape1()->crosses( l ) ) || ( getShape2()->crosses( l ) ) );
  }

  bool exclusion::crosses( const ray* l ) const
  {
    return ( ( getShape1()->crosses( l ) ) || ( getShape2()->crosses( l ) ) );
  }

  double exclusion::distance( const line* l ) const
  {
    double shortestDist = HUGE_VAL;

    stdexts::fifo< double > dists1 = getShape1()->distances( l );
    while ( dists1 )
    {
      double tmp = *dists1;
      if ( ( tmp >= 0.0 ) && tmp < shortestDist ) shortestDist = tmp;
    }

    stdexts::fifo< double > dists2 = getShape2()->distances( l );
    while ( dists2 )
    {
      double tmp = *dists2;
      if ( ( tmp >= 0.0 ) && tmp < shortestDist ) shortestDist = tmp;
    }
    return shortestDist;
  }

  stdexts::fifo< double > exclusion::distances( const line* l ) const
  {
    return ( getShape1()->distances( l ) << getShape2()->distances( l ) );
  }

  threeVector exclusion::intersect( const line* l ) const
  {
    return l->getStart() + this->distance( l )*l->getDirection();
  }

////////////////////////////////////////////////////////////////////////////////////////////////////

}

