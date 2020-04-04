
#ifndef __S3D_PATH_H__
#define __S3D_PATH_H__

#include "S3D_interaction.h"
#include "S3D_beam.h"

#include "stdexts.h"

#include <vector>


namespace S3D
{

  class pathvertex
  {
    private:
      interaction _theInteraction;
      double _weighting;
      beam _theBeam;

    public:
      pathvertex();
      pathvertex( interaction, double, beam );

      interaction getInteraction() const { return _theInteraction; }
      double getWeight() const { return _weighting; }
      beam getBeam() const { return _theBeam; }
  };

  typedef std::vector< pathvertex > path;

}

#endif // __S3D_PATH_H__

