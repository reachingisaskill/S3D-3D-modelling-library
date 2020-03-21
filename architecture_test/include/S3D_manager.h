
#ifndef __S3D__CONTROL_H__
#define __S3D__CONTROL_H__

#include "S3D_vector.h"
#include "S3D_rotation.h"
#include "S3D_beam.h"
#include "S3D_base.h"
#include "S3D_frame.h"
#include "S3D_raytracer.h"

#include <vector>
#include <set>
#include <map>


namespace S3D
{

  // Some declarations
  class printer;
//  class object_base;
  class volume_base;
  class light_interface;
  class camera_base;
  class frame;
  class rayTracer;


////////////////////////////////////////////////////////////////////////////////


  // TODO : Compiler flags for MULTITHREADED LIST
  // typedef stdexts::dll< object_base* > BasePointerContainerT;
  typedef std::set< object_base* > BasePointerContainerT;
  typedef std::map< int, BasePointerContainerT > PointersMapT;

  typedef std::set< object_3D_base* > Pointer3DContainerT;
  typedef std::map< int, Pointer3DContainerT > Object3DMapT;

  typedef std::set< light_interface* > LightingContainerT;
  typedef std::map< int, LightingContainerT > LightingMapT;


  class manager : public stdexts::singleton< manager >
  {
    friend class stdexts::singleton< manager >;
    friend class rayTracer;

    private:
      manager();

      // BasePointerContainerT _pointers;
      // Holds a pointer to every object
      PointersMapT _pointers;
      // Holders pointers to only objects with non-zero volume or suface area (excluding the world volume!)
      Object3DMapT _objects;
      // Holds pointers to the objects that emit light
      LightingMapT _lights;

      // Set of layers that are visible.
      std::set< int > _visibleLayers;

      std::map< int, colour > _defaultColours;

      bool _isVisibleLayer( int ) const;

      volume_base* _theWorld;
      camera_base* _theCamera;
      stdexts::autoPtr<frame> _lastFrame;
      std::string _printFile;
      unsigned int _numberObjects;

    public:
      virtual ~manager();

      void printLayer( int ) const; // Print a selected layer
      void printVisible() const; // Print all visible layers
      void printAll() const; // Print Everything!
      void setPrintFile( std::string );

      void setLayerVisible( int num ) { _visibleLayers.insert( num ); }
      void setLayerInvisible( int num ) { _visibleLayers.erase( num ); }
      bool getLayerVisible( int num ) { return this->_isVisibleLayer( num ); }

      void setDefaultColour( int num, colour c ) { _defaultColours[ num ] = c; }
      colour getDefaultColour( int num ) const;

      void setWorld( volume_base* );
      const object_base* getWorld() const;
      bool worldContains( threeVector ) const;

      void addObject( object_base*, int layer = 0);
      void removeObject( object_base* );

      void add3DObject( object_3D_base*, int layer = 0 );
      void remove3DObject( object_3D_base* );

      void addLight( light_interface*, int layer = 0 );
      void removeLight( light_interface* );

      void initObject( object_base* ) const;

      void setCamera( camera_base* );
      camera_base* theCamera() { return this->_theCamera; }
      const frame* getFrame();

      unsigned int countObjects() const;
  };


////////////////////////////////////////////////////////////////////////////////


  // TODO: remove these functions - will cause problems!
  template < class T >
  T* addObject( T* obj, int layer = 0 )
  {
    manager::getInstance()->addObject( (object_base*) obj, layer );

    return obj;
  }

  // TODO: remove these functions - will cause problems!
  template < class T >
  void removeObject( T* obj )
  {
    manager::getInstance()->removeObject( (object_base*) obj );
  }

}

#endif // __S3D__CONTROL_H__

