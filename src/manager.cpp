
#include "S3D_manager.h"

#include "S3D_base.h"
#include "S3D_volume.h"
#include "S3D_camera_base.h"

namespace S3D
{

  manager::manager() :
    _pointers(),
    _visibleLayers(),
    _theWorld( 0 ),
    _printFile( "" )
  {
  }

  manager::~manager()
  {
    for ( PointersMapT::iterator mapIt = _pointers.begin(); mapIt != _pointers.end(); ++mapIt )
    {
      for ( BasePointerContainerT::iterator it = mapIt->second.begin(); it != mapIt->second.end(); ++it )
      {
        delete (*it);
      }
      mapIt->second.clear();
    }
  }

  void manager::printLayer( int layerNum ) const
  {
    BasePointerContainerT::const_iterator itEnd = _pointers.find( layerNum )->second.end();
    for ( BasePointerContainerT::const_iterator it = _pointers.find( layerNum )->second.begin(); it != itEnd; ++it )
    {
      // TODO PRINT ME!
    }
  }

  void manager::printVisible() const
  {
    for ( PointersMapT::const_iterator mapIt = _pointers.begin(); mapIt != _pointers.end(); ++mapIt )
    {
      if ( ! _isVisibleLayer( mapIt->first ) ) continue;

      for ( BasePointerContainerT::const_iterator it = mapIt->second.begin(); it != mapIt->second.end(); ++it )
      {
        // TODO PRINT ME!
      }
    }
  }

  void manager::printAll() const
  {
    for ( PointersMapT::const_iterator mapIt = _pointers.begin(); mapIt != _pointers.end(); ++mapIt )
    {
      for ( BasePointerContainerT::const_iterator it = mapIt->second.begin(); it != mapIt->second.end(); ++it )
      {
        // TODO PRINT ME!
      }
    }
  }

  void manager::setPrintFile( std::string filename )
  {
    _printFile = filename;
  }

  colour manager::getDefaultColour( int num ) const
  {
    std::map< int, colour >::const_iterator iter = _defaultColours.find( num );

    if ( iter == _defaultColours.end() )
      return S3D_DEFAULT_COLOUR;
    else
      return iter->second;
  }

  void manager::setWorld( volume_base* obj )
  {
    if ( _theWorld )
    {
      this->removeObject( (object_base*) _theWorld );
    }

    this->addObject( (object_base*) obj, 0 );

    _theWorld = obj;
  }

  const object_base* manager::getWorld() const
  {
    return _theWorld;
  }

  bool manager::worldContains( threeVector p ) const
  {
    return _theWorld->contains( p );
  }

  void manager::addObject( object_base* obj, int layer )
  {
    if ( this->isAlive() )
    {
      if ( obj->_isOwned ) // See if it's alread been set!
      {
        _pointers[ obj->_layer ].erase( obj );
      }
      else
        obj->_isOwned = true;

      obj->_layer = layer;
      this->initObject( obj );
      _pointers[ layer ].insert( obj );
    }
  }

  void manager::removeObject( object_base* obj )
  {
    if ( this->isAlive() )
    {
      int layerNum = obj->getLayer();
      BasePointerContainerT::iterator found = _pointers[ layerNum ].find( obj );
      if ( found == _pointers[ layerNum ].end() )
      {
        return;
      }
      else
      {
        delete (*found);
        _pointers[ layerNum ].erase( found );
      }
    }
  }

  void manager::initObject( object_base* obj ) const
  {
    if ( this->isAlive() )
      obj->_colour = getDefaultColour( obj->_layer );
  }

  unsigned int manager::countObjects() const
  {
    unsigned int counter = 0;
    for ( PointersMapT::const_iterator mapIt = _pointers.begin(); mapIt != _pointers.end(); ++mapIt )
    {
      counter += mapIt->second.size();
    }
    return counter;
  }


  bool manager::_isVisibleLayer( int layerNum ) const
  {
    std::set< int >::const_iterator iter = _visibleLayers.find( layerNum );
    
    if ( iter == _visibleLayers.end() )
      return false;
    else
      return true;
  }

  void manager::setCamera( camera_base* c )
  {
    if ( _theCamera != nullptr )
    {
    }
    _theCamera = c;
  }

  const frame* manager::getFrame()
  {
    if ( _theCamera == nullptr )
    {
      _lastFrame = stdexts::autoPtr<frame>(nullptr);
    }
    else 
    {
      _theCamera->shutter();
      _lastFrame = _theCamera->popFrame();
    }
    return _lastFrame.get();
  }

}

