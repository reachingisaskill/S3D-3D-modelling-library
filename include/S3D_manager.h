
#ifndef __S3D__CONTROL_H__
#define __S3D__CONTROL_H__

#include "S3D_defs.h"
#include "S3D_global.h"


#include <vector>
#include <set>
#include <map>


namespace S3D
{

  // Some declarations
  class printer;
  class object_base;
  class volume_base;
  class camera_base;
  class frame;


////////////////////////////////////////////////////////////////////////////////


  // TODO : Compiler flags for MULTITHREADED LIST
  // typedef stdexts::dll< object_base* > BasePointerContainerT;
  typedef std::set< object_base* > BasePointerContainerT;
  typedef std::map< int, BasePointerContainerT > PointersMapT;


  class manager : public stdexts::singleton< manager >
  {
    friend class stdexts::singleton< manager >;

    private:
      manager();

      // BasePointerContainerT _pointers;
      PointersMapT _pointers;
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

      void addObject( object_base*, int );
      void removeObject( object_base* );

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

