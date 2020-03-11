
#include "S3D_objectContainers.h"


namespace S3D
{

  trail::trail( threeVector p ) :
    line( p, threeVector( 0.0 ) ),
    _data()
  {
    _data.push_back( p );
  }

  trail::~trail()
  {
  }

  void trail::addPoint( threeVector p )
  {
    _data.push_back( p );
  }

  /*
  print_base* trail::_print() const
  {
    print_lineSet* newShape = new print_lineSet( "IndexedLineSet" );
    for ( std::vector< threeVector >::const_iterator it = _data.begin(); it != _data.end(); ++it )
      newShape->addPoint( *it );

    return newShape;
  }
  */

}

