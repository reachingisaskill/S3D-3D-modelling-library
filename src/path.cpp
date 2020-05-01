
#include "S3D_path.h"


namespace S3D
{

  pathvertex::pathvertex() :
    _theInteraction(),
    _throughput(),
    _weight( 1.0 )
  {
  }


  pathvertex::pathvertex( interaction inter, spectrum b, double weight ) :
    _theInteraction( inter ),
    _throughput( b ),
    _weight( weight )
  {
  }

//////////////////////////////////////////////////////////////////////////////////////////////////////
//  // Path
//
//  path::path() :
//    _thePath()
//  {
//  }
//
//
//  void path::push_back( pathvertex v )
//  {
//    _thePath.push_back( v );
//  }


}

