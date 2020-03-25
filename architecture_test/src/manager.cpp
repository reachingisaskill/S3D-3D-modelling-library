
#include "S3D_manager.h"

#include "logtastic.h"

namespace S3D
{

  manager::manager() :
    _theWorld( nullptr ),
    _theCamera( nullptr ),
    _pointers(),
    _objects(),
    _lights(),
    _materials(),
    _inVisibleLayers(),
    _lastFrame()
  {
    INFO_LOG( "S3D Manager instantiated" );
  }

  manager::~manager()
  {
    INFO_LOG( "S3D Destroying S3D Manager" );
    // Remove pointers to objects
    for ( ObjectMapT::iterator mapIt = _objects.begin(); mapIt != _objects.end(); ++mapIt )
    {
      mapIt->second.clear();
    }
    INFO_LOG( "Removed Object pointers" );

    // Remove pointers to lights
    for ( LightMapT::iterator mapIt = _lights.begin(); mapIt != _lights.end(); ++mapIt )
    {
      mapIt->second.clear();
    }
    INFO_LOG( "Removed Lights pointers" );

    // Remove pointers to materials
    _materials.clear();
    INFO_LOG( "Removed Materials pointers" );

    // Delete all objects in memory
    for ( BaseContainerT::iterator it = _pointers.begin(); it != _pointers.end(); ++it )
    {
      delete (*it);
    }
    _pointers.clear();
    INFO_LOG( "All objects deleted" );
  }


  bool manager::_isVisibleLayer( int layerNum ) const
  {
    std::set< int >::const_iterator iter = _inVisibleLayers.find( layerNum );
    
    if ( iter != _inVisibleLayers.end() )
      return false;
    else
      return true;
  }


  void manager::_addBase( base* obj )
  {
    INFO_STREAM << "Taking ownership of base pointer: " << obj;
    if ( _pointers.find( obj ) != _pointers.end() )
    {
      INFO_LOG( "Base pointer already stored in memory set" );
      obj->_isOwned = true;
    }
    else
    {
      _pointers.insert( obj );
      obj->_isOwned = true;
    }
  }


  void manager::_removeBase( base* obj )
  {
    INFO_STREAM << "Removing base pointer: " << obj;
    if ( _pointers.find( obj ) == _pointers.end() )
    {
      WARN_LOG( "Pointer not found in memory map" );
      obj->_isOwned = false;
    }
    else
    {
      _pointers.erase( obj );
      obj->_isOwned = false;
      delete obj;
    }
  }


  void manager::setWorld( object_base* obj, int layerNum )
  {
    INFO_STREAM <<  "Setting world volume: " << (base*) obj;
    if ( _theWorld != nullptr )
    {
      WARN_LOG( "World volume already set - removing." );
      this->removeObject( _theWorld );
    }

    this->addObject( obj, layerNum );
    _theWorld = obj;
  }


  const object_base* manager::getWorld() const
  {
    return _theWorld;
  }


  bool manager::worldContains( point p ) const
  {
    return _theWorld->contains( p );
  }


  void manager::addObject( object_base* obj, int layer )
  {
    INFO_STREAM << "Adding object: " << (base*) obj;
    if ( obj->_isOwned ) // See if it's already owned
    {
      INFO_LOG( "Object already owned. Setting layer number." );
      _objects[ obj->_layer ].erase( obj );
      obj->_layer = layer;
      _objects[ obj->_layer ].insert( obj );
    }
    else
    {
      _addBase( (base*) obj );
      obj->_layer = layer;
      _objects[ obj->_layer ].insert( obj );
    }
  }


  void manager::removeObject( object_base* obj )
  {
    INFO_STREAM << "Removing object: " << (base*) obj;
    if ( ! obj->_isOwned )
    {
      WARN_LOG( "Object is not owned." );
      return;
    }

    _objects[ obj->_layer ].erase( obj ); // Delete the pointer in the object list
    _removeBase( (base*) obj );
  }



  void manager::addLight( light_base* obj, int layer )
  {
    INFO_STREAM << "Added light: " << (base*) obj;
    if ( obj->_isOwned ) // See if it's already owned
    {
      INFO_LOG( "Light is already owned. Setting layer." );
      _lights[ obj->_layer ].erase( obj );
      obj->_layer = layer;
      _lights[ obj->_layer ].insert( obj );
    }
    else
    {
      _addBase( (base*) obj );
      obj->_layer = layer; // Set layer
      _lights[ obj->_layer ].insert( obj ); // Add pointer to lights map.
    }
  }


  void manager::removeLight( light_base* obj )
  {
    INFO_STREAM << "Removing light: " << (base*) obj;
    if ( obj->_isOwned )
    {
      _removeBase( (base*) obj );
      _lights[ obj->_layer ].erase( obj ); // Delete the pointer in the light list
    }
    else
    {
      WARN_LOG( "Object is not owned." );
    }
  }


  void manager::addMaterial( std::string name, material_base* obj )
  {
    INFO_STREAM << "Adding '" << name << "' as a material: " << (base*) obj;
    if ( obj->_isOwned )
    {
      INFO_LOG( "Material is already owned." );
      _materials[name] = obj;
    }
    else
    {
      _addBase( (base*) obj );
      _materials[name] = obj;
    }
  }


  const material_base* manager::getMaterial( std::string name ) const
  {
    MaterialMapT::const_iterator found = _materials.find( name );
    if ( found == _materials.end() )
    {
      WARN_STREAM << "Material '" << name << "' was not found. Return nullptr.";
      return nullptr;
    }
    else
    {
      return found->second;
    }
  }


  void manager::removeMaterial( std::string name )
  {
    INFO_STREAM << "Attemping to remove material named: '" << name << "'.";
    MaterialMapT::const_iterator found = _materials.find( name );
    if ( found == _materials.end() )
    {
      INFO_LOG( "Material not found." );
      return;
    }
    else
    {
      material_base* obj = found->second;
      _materials.erase( found );
      _removeBase( (base*) obj );
    }
  }


  unsigned int manager::countObjects() const
  {
    return _pointers.size();
  }


  void manager::setCamera( camera_base* c )
  {
    INFO_STREAM << "Setting camera: " << (base*) c;
    if ( _theCamera != nullptr )
    {
      INFO_STREAM << "Camera, " << (base*) _theCamera << ", already set - removing.";
      _removeBase( (base*) _theCamera );
    }
    _theCamera = c;
  }


  const frame* manager::getFrame()
  {
    if ( _theCamera == nullptr )
    {
      WARN_LOG( "No camera set. Returning empty frame." );
      _lastFrame = stdexts::autoPtr<frame>(nullptr);
    }
    else 
    {
      INFO_LOG( "Taking frame." );
      _theCamera->shutter();
      _lastFrame = _theCamera->popFrame();
    }
    return _lastFrame.get();
  }

}

