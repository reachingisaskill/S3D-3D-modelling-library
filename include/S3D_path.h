
#ifndef __S3D_PATH_H__
#define __S3D_PATH_H__

#include "S3D_interaction.h"
#include "S3D_spectrum.h"

#include "stdexts.h"

#include <vector>


namespace S3D
{

  class pathvertex
  {
    private:
      interaction _theInteraction;
      double _weighting;
      spectrum _theBeam;

    public:
      pathvertex();
      pathvertex( interaction, double, spectrum );

      interaction getInteraction() const { return _theInteraction; }
      double getWeight() const { return _weighting; }
      spectrum getBeam() const { return _theBeam; }
  };

  typedef std::vector< pathvertex > path;

}

#endif // __S3D_PATH_H__

