
#ifndef __S3D__TRACER_BASE_H__
#define __S3D__TRACER_BASE_H__

#include "S3D_vector.h"
#include "S3D_point.h"
#include "S3D_line.h"
#include "S3D_spectrum.h"
#include "S3D_interaction.h"
#include "S3D_manager.h"

#include <set>

namespace S3D
{

  // Using typedefs as will probably switch to acceleration structures in the future
  typedef ObjectContainerT ObjectListT;
  typedef ObjectContainerT LightListT;


  class tracer_base
  {
    private:
      std::set< int > _layers;
      double _lightSamplesPerArea;
      double _totalLightArea;
      unsigned int _numLightSamples;

      ObjectListT _objectList;
      LightListT _lightList;

    protected:
      tracer_base();

      const ObjectListT& _getObjects() const { return _objectList; }

      const LightListT& _getLights() const { return _lightList; }

      interaction getInteraction( line& l );

      const object_base* _chooseLight() const;

      // Override in derived classes to be called during setup proceedure.
      virtual void _additionalSetup() {}

    public:
      virtual ~tracer_base() {}

      // Loads the object and light caches in to the tracing object.
      virtual void setup();

      // Called by the camera - primary interface to ray tracing
      virtual spectrum traceRay( point start, threeVector direction ) = 0;

      // Estimate the light emitted from and object that is seen at an interaction point.
      virtual spectrum sampleLight( const object_base*, const interaction& );

      // Estimate the light received at an interaction point from all sources of light in the scene.
      // No. Samples, interaction vertex
      virtual spectrum sampleAllLights( const interaction& );

      // Trace a "shadow ray" or a specific light ray from a light source
      virtual spectrum traceLightSample( spectrum, point, const interaction& );

      // Can one point directly see anpther?
      virtual bool isVisible( point start, point end, threeVector Normal ) const;
      

      // Total area of all light emitting surfaces.
      double getTotalLightArea() const { return _totalLightArea; }

      // Choose the layers to render
      void addLayer( int );
      void removeLayer( int );

      // Set the number of light samples per unit area
      void setLightSampleRate( double s ) { _lightSamplesPerArea = s; }
  };
}

#endif // __S3D__TRACER_BASE_H__

