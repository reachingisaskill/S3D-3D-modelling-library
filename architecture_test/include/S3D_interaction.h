
#ifndef __S3D__INTERACTION_H__
#define __S3D__INTERACTION_H__

#include "S3D_vector.h"
#include "S3D_line.h"
#include "S3D_surface.h"
#include "S3D_object_base.h"
#include "S3D_texture.h"

#include "stdexts.h"

namespace S3D
{
  class object_base;

  // Stores the information describing an interaction between a light beam and a surface.
  class interaction
  {
    private:
      point _thePoint; // Interaction point
      const line* _theLine; // Line traced from previous point
      const object_base* _theObject; // Object intersected by line
      double _distance; // Distance traveled by beam
      double _distanceSq; // Distance traveled by beam squared
      surfacemap _surfacemap;

      threeVector _surfaceNormal; // Surface normal at interaction
      double _refIndexRatio;

    public :
      interaction();

      // Point of interaction, line from viewer, object hit, surface normal vector, ratio of refractive indices ( n_1 / n_2 )
      interaction( point, const line*, const object_base*, threeVector, double );

      const point& getPoint() const { return this->_thePoint; }

      const line& getLine() const { return *this->_theLine; }

      const object_base* getObject() const { return this->_theObject; }

      const threeVector& getSurfaceNormal() const { return this->_surfaceNormal; }

      double getDistance() const { return this->_distance; }

      double getDistanceSquared() const { return this->_distanceSq; }

      surfacemap getSurfaceMap() const { return this->_surfacemap; }

      double getIndexRatio() const { return _refIndexRatio; }

      threeVector getTransmission() const;

      threeVector getReflection() const;
  };

}

#endif // __S3D__INTERACTION_H__

