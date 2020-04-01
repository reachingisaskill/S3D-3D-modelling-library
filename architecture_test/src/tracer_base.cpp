#define __DEBUG_OFF__

#include "S3D_tracer_base.h"

#include "S3D_defs.h"

#include "logtastic.h"


namespace S3D
{

  tracer_base::tracer_base() :
    _layer( 0 )
  {
  }


  void tracer_base::setup()
  {
    ObjectMapT& objectMap = manager::getInstance()->_objects;
    if ( objectMap.find( _layer ) == objectMap.end() )
    {
      WARN_LOG( "tracer_base::_getObjects() : Specified layer is empty. No objects will be rendered." );
      _objectList = ObjectListT();
    }
    else
    {
      _objectList = objectMap[_layer];
    }

    LightMapT& lights = manager::getInstance()->_lights;
    if ( lights.find( _layer ) == lights.end() )
    {
      WARN_LOG( "tracer_base::_getLights() : Specified layer is empty. No lights will be rendered." );
      _lightList = LightListT();
    }
    else
    {
      _lightList = lights[_layer];
    }

    // Any other setup info?
  }


  beam tracer_base::traceLightSample( beam b, point p, const interaction& inter ) const
  {
    if ( isVisible( p, inter.getPoint() ) )
    {
      threeVector lightDir = (inter.getPoint() - p).norm();
      return inter.getObject()->getMaterial()->scatter( lightDir, b, inter );
    }
    else 
    {
      return beam( 0.0, 0.0, 0.0 );
    }
  }


  bool tracer_base::isVisible( point start, point end ) const
  {
    threeVector separation = end - start;
    line the_beam( start, separation );
    double distanceSq = separation.square();

    const ObjectListT& obj_list = _getObjects();
    ObjectListT::const_iterator obj_it = obj_list.begin();
    ObjectListT::const_iterator obj_end = obj_list.end();

    while( obj_it != obj_end )
    {
      if ( (*obj_it)->crosses( the_beam ) )
      {
        interaction intersect = (*obj_it)->intersect( the_beam );

//        if ( ( distance - intersect.getDistance() ) > epsilon )
        if ( ( distanceSq - intersect.getDistanceSquared() ) > epsilon )
        {
          return false;
        }
      }
      ++obj_it;
    }
    return true;
  }


  interaction tracer_base::getInteraction( line& l ) const
  {
    const ObjectListT& objects = _getObjects();
    ObjectListT::const_iterator obj_it = objects.begin();
    ObjectListT::const_iterator obj_end = objects.end();

    interaction current_intersect;

    try
    {
      DEBUG_STREAM << "Tracing ray from: " << l.getStart().getPosition() << " with direction: " << l.getDirection();
      while( obj_it != obj_end )
      {
        if ( (*obj_it)->crosses( l ) )
        {
          interaction intersect = (*obj_it)->intersect( l );
          double distanceSq = intersect.getDistanceSquared();

          if ( ( distanceSq > epsilon ) && ( distanceSq < current_intersect.getDistanceSquared() ) )
          {
            current_intersect = intersect;
          }
        }

        ++obj_it;
      }
    }
    catch( stdexts::exception& e )
    {
      std::stringstream ss;
      ss << "Tracing ray from: " << l.getStart().getPosition() << " with direction: " << l.getDirection();
      EX_LOG( e, ss.str() );
      RETHROW( e );
    }

    return current_intersect;
  }

}

