
#ifndef __S3D__TRACER_BASE_H__
#define __S3D__TRACER_BASE_H__

#include "S3D_manager.h"


namespace S3D
{

  // Using typedefs as will probably switch to acceleration structures in the future
  typedef ObjectContainerT ObjectListT;
  typedef LightContainerT LightListT;


  class tracer_base
  {
    private:
      int _layer;
      ObjectListT _objectList;
      LightListT _lightList;

    protected:
      tracer_base();

      const ObjectListT& _getObjects() const { return _objectList; }

      const LightListT& _getLights() const { return _lightList; }

      interaction getInteraction( line& l ) const;

    public:
      virtual ~tracer_base() {}

      // Loads the object and light caches in to the tracing object.
      virtual void setup();

      // Called by the camera - primary interface to ray tracing
      virtual beam traceRay( point start, threeVector direction ) const = 0;

      // Trace a "shadow ray" or a specific light ray from a light source
      virtual beam traceLightSample( beam, point, const interaction& ) const;

      // Can one point directly see anpther?
      virtual bool isVisible( point start, point end ) const;
      

      // Set the layer to render
      void setLayerNum( int l ) { _layer = l; }
  };
}

#endif // __S3D__TRACER_BASE_H__

