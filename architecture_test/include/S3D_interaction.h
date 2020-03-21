
#ifndef __S3D__INTERACTION_H__
#define __S3D__INTERACTION_H__

#include "S3D_vector.h"
#include "S3D_line.h"
#include "S3D_surface.h"
#include "S3D_surfacemapping.h"

namespace S3D
{

  // Stores the information describing an interaction between a light beam and a surface.
  class interaction
  {
    private:
      threeVector _thePoint;
      const line* _theLine;
      const object_base* _theObject;
      threeVector _surfaceNormal;
      const surfaceMapping* _surfaceMapping;
      double _cosIncidentAngle;

    public :
      interaction( threeVector, const line*, const object_base*, threeVector, const surfaceMapping* );

      const line* getLine() const { return this->_theLine; }

      const object_base* getObject() const { return this->_theObject; }

      threeVector getSurfaceNormal() const { return this->_surfaceNormal; }

      const surfaceMapping* getSurfaceMapping() const { return this->_surfaceMapping; }

      double getCosine() const { return this->_cosIncidentAngle; }

  };
}

#endif // __S3D__INTERACTION_H__

