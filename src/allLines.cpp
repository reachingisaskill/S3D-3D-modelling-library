
#include "S3D_allLines.h"


namespace S3D
{

  ray::ray( threeVector s, threeVector e ) :
    line( s, _calcGrad( s, e ) ),
    _end( e ),
    _length( ( e - s ).mod() )
  {
  }

  ray::ray( threeVector p, rotation r, double l ) :
    line( p, r ),
    _end( p + this->getDirection() * l ),
    _length( l )
  {
  }

  ray::ray( threeVector p, threeVector v, double len ) :
    line( p, v ),
    _end( p + len*v ),
    _length( len )
  {
  }

  threeVector ray::_calcGrad( const threeVector& v1, const threeVector& v2 )
  {
    threeVector temp = v2 - v1;
    return temp * ( 1.0 / temp.mod() );
  }

  void ray::setLength( double len )
  {
    _end = this->getStart() + ( this->getDirection() * len );
  }

  /*
  print_base* ray::_print() const
  {
    print_lineSet* newShape = new print_lineSet( "IndexedLineSet" );
    newShape->addPoint( this->start() );
    newShape->addPoint( this->end() );

    return newShape;
  }
  */



///////////////////////////////////////////////////////////////////////////////


  spline::spline( threeVector p, threeVector v ) :
    line( p, v )
  {
  }

  /*
  double spline::getLength() const
  {
  }
  */

  void spline::addPoint( threeVector p, threeVector v )
  {
    splinePoint newPoint = { p, v };
    _points.push_back( newPoint );
  }

  /* 
  print_base* spline::_print() const
  {
  }
  */

}

