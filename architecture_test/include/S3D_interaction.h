
#ifndef __S3D__INTERACTION_H__
#define __S3D__INTERACTION_H__

#include "S3D_vector.h"
#include "S3D_line.h"
#include "S3D_surface.h"
#include "S3D_object_base.h"

#include "stdexts.h"

namespace S3D
{
  class object_base;

  // Stores the information describing an interaction between a light beam and a surface.
  class interaction
  {
    private:
      point _thePoint;
      const line* _theLine;
      const object_base* _theObject;
      threeVector _surfaceNormal;
      double _distance;

    public :
      interaction();

      // Point of interaction, line from viewer, object hit, surface normal vector
      interaction( point, const line*, const object_base*, threeVector );

      const point& getPoint() const { return this->_thePoint; }

      const line& getLine() const { return *this->_theLine; }

      const object_base* getObject() const { return this->_theObject; }

      const threeVector& getSurfaceNormal() const { return this->_surfaceNormal; }

      double getDistance() const { return this->_distance; }
  };
}

#endif // __S3D__INTERACTION_H__

