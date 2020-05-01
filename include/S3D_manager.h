
#ifndef S3D_CONTROL_H_
#define S3D_CONTROL_H_

#include "S3D_vector.h"
#include "S3D_rotation.h"
#include "S3D_spectrum.h"
#include "S3D_base.h"
#include "S3D_object_base.h"
#include "S3D_material_base.h"
#include "S3D_camera_base.h"
#include "S3D_light_base.h"
#include "S3D_frame.h"

#include <vector>
#include <set>
#include <map>


namespace S3D
{

  // Forward Declarations
  class tracer_base;


  // Typedefs for later
  typedef std::set< base* > BaseContainerT;

  typedef std::set< object_base* > ObjectContainerT;
  typedef std::map< int, ObjectContainerT > ObjectMapT;

  typedef std::set< light_base* > LightContainerT;
  typedef std::map< int, LightContainerT > LightMapT;

  typedef std::map< std::string, material_base* > MaterialMapT;


  // Manager Class definition
  class manager : public stdexts::singleton< manager >
  {
    friend class stdexts::singleton< manager >;
    friend class tracer_base;

    private:
      manager();

      object_base* _theWorld;
      camera_base* _theCamera;

      // Holds a pointer to every object - this set own all the memory.
      BaseContainerT _pointers;
      // Holders pointers to objects with non-zero volume or suface area (excluding the world volume!)
      ObjectMapT _objects;
      // Holds pointers to the objects that emit light
      LightMapT _lights;
      // Holds pointers to all the material definitions
      MaterialMapT _materials;

      // Set of layers that are invisible. (Default is visible)
      std::set< int > _inVisibleLayers;

      stdexts::autoPtr<frame> _lastFrame;

      unsigned int _lightSamplesPerArea;
      spectrum _ambientLight;

      void _addBase( base* );
      void _removeBase( base* );
      bool _isVisibleLayer( int ) const;

    public:
      virtual ~manager();

      void setLayerVisible( int num ) { _inVisibleLayers.erase( num ); }
      void setLayerInvisible( int num ) { _inVisibleLayers.insert( num ); }
      bool getLayerVisible( int num ) const { return ! this->_isVisibleLayer( num ); }

      void setWorld( object_base*, int = 0 );
      const object_base* getWorld() const;
      bool worldContains( point ) const;

      void setAmbientLight( spectrum b ) { _ambientLight = b; }
      spectrum getAmbientLight() const { return _ambientLight; }

      void setLightSampleRate( unsigned int i ) { _lightSamplesPerArea = i; }
      unsigned int getLightSampleRate() const { return _lightSamplesPerArea; }


      void addObject( object_base*, int layer = 0);
      void removeObject( object_base* );


      void addLight( light_base*, int layer = 0 );
      void removeLight( light_base* );


      void addMaterial( std::string, material_base* );
      material_base* getMaterial( std::string );
      void removeMaterial( std::string );


      void setCamera( camera_base* );
      camera_base* theCamera() { return this->_theCamera; }
      const frame* getFrame();

      unsigned int countObjects() const;
  };

}

#endif // S3D_CONTROL_H_

