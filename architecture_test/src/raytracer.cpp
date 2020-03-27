#define __DEBUG_OFF__

#include "S3D_raytracer.h"

#include "S3D_defs.h"
#include "S3D_line.h"
#include "S3D_manager.h"
#include "S3D_light_base.h"
#include "S3D_interaction.h"

#include "logtastic.h"

#include <sstream>


namespace S3D
{

  rayTracer::rayTracer() :
    _layer( 0 ),
    _currentBeam()
  {
  }

  rayTracer::~rayTracer()
  {
  }

  beam rayTracer::traceRay( point start, threeVector dir )
  {
    line the_ray( start, dir );
    _currentBeam = beam();

    ObjectMapT& objectMap = manager::getInstance()->_objects;
    ObjectContainerT::iterator obj_it = objectMap[_layer].begin();
    ObjectContainerT::iterator obj_end = objectMap[_layer].end();

    interaction current_intersect;

    try
    {
      DEBUG_STREAM << "Tracing ray from: " << start.getPosition() << " with direction: " << dir;
      while( obj_it != obj_end )
      {
        if ( (*obj_it)->crosses( the_ray ) )
        {
          interaction intersect = (*obj_it)->intersect( the_ray );

          if ( intersect.getDistance() < current_intersect.getDistance() )
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
      ss << "Tracing ray from camera: " << start.getPosition() << " with direction: " << dir;
      EX_LOG( e, ss.str() );
      std::cerr << ELUCIDATE( e );
      return beam();
    }


      if ( current_intersect.getObject() == nullptr )
      {
        DEBUG_LOG( "No intersecting object found. Returning empty beam" );
        return beam();
      }

  //    colour c( current_intersect.getObject()->getMaterial()->getColour( current_intersect ) );
  //    return beam( c, 1.0 );
      
      DEBUG_LOG( "Tracing Lights." );
      DEBUG_STREAM << "Interaction: " << current_intersect.getPoint().getPosition() << " | " << current_intersect.getLine().getStart().getPosition() << " - " << current_intersect.getLine().getDirection();

      LightMapT& lights = manager::getInstance()->_lights;
      LightContainerT::iterator light_it = lights[_layer].begin();
      LightContainerT::iterator light_end = lights[_layer].end();


      try
      {
        while( light_it != light_end )
        {
          DEBUG_LOG( "Sampling Light Source" );
          (*light_it)->sampleRays( current_intersect, this );
          DEBUG_STREAM << " Current Beam: " << _currentBeam.red() << ", " << _currentBeam.green() << ", " << _currentBeam.blue();
          ++light_it;
        }
      }
      catch( stdexts::exception& e )
      {
      std::stringstream ss;
      ss << "Tracing ray from light source: " << start.getPosition() << " with direction: " << dir;
      EX_LOG( e, ss.str() );
      std::cerr << ELUCIDATE( e );
      return beam();
      }

      DEBUG_STREAM << "Returning Current Beam." << _currentBeam.red() << ", " << _currentBeam.green() << ", " << _currentBeam.blue();
      return _currentBeam;
  }


  void rayTracer::traceLightSample( beam b, point p, const interaction& inter )
  {
    if ( isVisible( p, inter.getPoint() ) )
    {
      threeVector lightDir = (inter.getPoint() - p).norm();
      _currentBeam += inter.getObject()->getMaterial()->scatter( lightDir, b, inter );
    }
    else DEBUG_LOG( "SHADOW!" );
  }


  bool rayTracer::isVisible( point start, point end ) const
  {
    threeVector separation = end - start;
    line the_beam( start, separation );
    double distance = separation.mod();


    ObjectMapT& objectMap = manager::getInstance()->_objects;
    ObjectContainerT::iterator obj_it = objectMap[_layer].begin();
    while( obj_it != objectMap[_layer].end() )
    {
      if ( (*obj_it)->crosses( the_beam ) )
      {
        interaction intersect = (*obj_it)->intersect( the_beam );

//        if ( ( intersect.getDistance() - distance ) < epsilon )
        if ( ( distance - intersect.getDistance() ) > epsilon )
        {
          return false;
        }
      }
      ++obj_it;
    }
    return true;
  }

}

