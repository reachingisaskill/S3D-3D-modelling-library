//#define __DEBUG_OFF__

#include "S3D_recursiveraytracer.h"

#include "S3D_defs.h"
#include "S3D_line.h"
#include "S3D_manager.h"
#include "S3D_light_base.h"
#include "S3D_interaction.h"
#include "S3D_random.h"

#include "logtastic.h"

#include <sstream>


namespace S3D
{

  tracer_recursive::tracer_recursive() :
    _maxDepth( 1 )
  {
  }

  tracer_recursive::~tracer_recursive()
  {
  }


  beam tracer_recursive::_traceRay( point start, threeVector dir, unsigned int depth )
  {
    line the_ray( start, dir );

    const ObjectListT& objects = _getObjects();
    ObjectListT::const_iterator obj_it = objects.begin();
    ObjectListT::const_iterator obj_end = objects.end();

    interaction current_intersect;

    try
    {
      DEBUG_STREAM << "Tracing ray from: " << start.getPosition() << " with direction: " << dir << " -- Depth = " << depth;
      while( obj_it != obj_end )
      {
        if ( (*obj_it)->crosses( the_ray ) )
        {
          interaction intersect = (*obj_it)->intersect( the_ray );
//          double distance = intersect.getDistance();
          double distanceSq = intersect.getDistanceSquared();

          if ( ( distanceSq > epsilon ) && ( distanceSq < current_intersect.getDistanceSquared() ) )
//          if ( ( distance > epsilon ) && ( distance < current_intersect.getDistance() ) )
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
      ss << "Tracing ray from: " << start.getPosition() << " with direction: " << dir << " -- Depth = " << depth;
      EX_LOG( e, ss.str() );
      std::cerr << ELUCIDATE( e );
      return beam( 0.0, 0.0, 0.0 );
    }


    if ( current_intersect.getObject() == nullptr )
    {
      DEBUG_LOG( "No intersecting object found, returning." );
      return beam( 0.0, 0.0, 0.0 );
    }


    DEBUG_STREAM << "Interaction: " << current_intersect.getDistance() << " -- " << current_intersect.getPoint().getPosition() << " | " << current_intersect.getLine().getStart().getPosition() << " - " << current_intersect.getLine().getDirection();

    beam currentBeam( 0.0, 0.0, 0.0 );

    if ( depth < _maxDepth )
    {
      DEBUG_LOG( "Checking for recursive rays." );

      double prob_t = current_intersect.getObject()->getMaterial()->getTransmissionProb( current_intersect );
      double prob_r = current_intersect.getObject()->getMaterial()->getReflectionProb( current_intersect );

      if ( prob_t > epsilon )
      {
        DEBUG_LOG( "Launching recursive transmitted ray." );
        currentBeam += prob_t*this->_traceRay( current_intersect.getPoint(), current_intersect.getTransmission(), depth + 1 );
      }

      if ( prob_r > epsilon )
      {
        DEBUG_LOG( "Launching recursive reflected ray." );
        currentBeam += prob_r*this->_traceRay( current_intersect.getPoint(), current_intersect.getReflection(), depth + 1 );
      }

    }
    else
    {
      DEBUG_LOG( "Max depth reached closing recursion." );
    }




    DEBUG_LOG( "Tracing Lights." );
    const LightListT& lights = _getLights();
    LightListT::const_iterator light_it = lights.begin();
    LightListT::const_iterator light_end = lights.end();

    try
    {
      while( light_it != light_end )
      {
        DEBUG_LOG( "Sampling Light Source" );
//        currentBeam += (*light_it)->sampleRays( current_intersect, this );
        currentBeam += sampleLight( (*light_it), current_intersect );
        DEBUG_STREAM << " Current Beam: " << currentBeam.red() << ", " << currentBeam.green() << ", " << currentBeam.blue();
        ++light_it;
      }
      DEBUG_LOG( "Estimating ambient component" );
      currentBeam += manager::getInstance()->getAmbientLight() * current_intersect.getObject()->getMaterial()->getColour( current_intersect.getSurfaceMap() );
      DEBUG_STREAM << " Current Beam: " << currentBeam.red() << ", " << currentBeam.green() << ", " << currentBeam.blue();
    }
    catch( stdexts::exception& e )
    {
      std::stringstream ss;
      ss << "Tracing ray from light source: " << start.getPosition() << " with direction: " << dir;
      EX_LOG( e, ss.str() );
      std::cerr << ELUCIDATE( e );
      return beam( 0.0, 0.0, 0.0 );
    }

    DEBUG_STREAM << "Returning Current Beam." << currentBeam.red() << ", " << currentBeam.green() << ", " << currentBeam.blue();
    return currentBeam;
  }


  beam tracer_recursive::traceRay( point start, threeVector dir )
  {
    return this->_traceRay( start, dir ); // Starts the recursive function
  }

}

