
#ifndef __S3D__OBJECT_CONTAINERS_H__
#define __S3D__OBJECT_CONTAINERS_H__


#include "S3D_global.h"
#include "S3D_defs.h"

#include "S3D_allPoints.h"
#include "S3D_allLines.h"
#include "S3D_allSurfaces.h"
#include "S3D_allShapes.h"


namespace S3D
{

  class trail : public line
  {
    private:
      std::vector< threeVector > _data;

    protected:
      // virtual print_base* _print() const;

    public:
      trail( threeVector );
      virtual ~trail();

      void addPoint( threeVector );
  };

}

#endif // __S3D__OBJECT_CONTAINERS_H__

