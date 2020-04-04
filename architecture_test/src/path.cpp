
#include "S3D_path.h"


namespace S3D
{

  pathvertex::pathvertex() :
    _theInteraction(),
    _weighting(),
    _theBeam()
  {
  }


  pathvertex::pathvertex( interaction inter, double weight, beam b ) :
    _theInteraction( inter ),
    _weighting( weight ),
    _theBeam( b )
  {
  }


}

