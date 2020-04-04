#define __DEBUG_OFF__

#include "S3D_pathtracer.h"

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

  tracer_pathtracer::tracer_pathtracer() :
    _maxDepth( 1 ),
    _killProb( 0.5 )
  {
  }


  tracer_pathtracer::~tracer_pathtracer()
  {
  }


  void tracer_pathtracer::setKillProb( double k )
  {
    if ( k > 1.0 )
    {
      WARN_LOG( "Kill probability > 1.0. Paths will not propagate" );
      k = 1.0;
    }
    if ( k < 0.0 )
    {
      WARN_LOG( "Kill probability < 0.0. Setting to zero." );
      k = 0.0;
    }

    _killProb = k;
  }


  beam tracer_pathtracer::_pathTrace( point start, threeVector dir, unsigned int depth )
  {
    line the_ray( start, dir );
    double kill_factor = ( depth < 5 ? 0.0 : _killProb );
    double roulette_factor = 1.0 / ( 1.0 - kill_factor );

    interaction current_intersect;
    try
    {
      current_intersect = this->getInteraction( the_ray );
    }
    catch( stdexts::exception& e )
    {
      std::stringstream ss;
      ss << "Path Tracer Depth = " << depth;
      EX_LOG( e, ss.str() );
      std::cerr << ELUCIDATE( e );
      return beam( 0.0, 0.0, 0.0 );
    }

    if ( current_intersect.getObject() == nullptr )
    {
      return beam( 0.0, 0.0, 0.0 );
    }


    DEBUG_STREAM << "Interaction: " << current_intersect.getDistance() << " -- " << current_intersect.getPoint().getPosition() << " | " << roulette_factor;

    beam currentBeam( 0.0, 0.0, 0.0 );

    if ( ( depth < _maxDepth ) && ( random::uniformDouble() > _killProb ) )
    {
      DEBUG_LOG( "Checking for recursive rays." );

      double r1 = random::uniformDouble();

      double prob_r = current_intersect.getObject()->getMaterial()->getReflectionProb( current_intersect );
      double prob_t = current_intersect.getObject()->getMaterial()->getTransmissionProb( current_intersect );

      if ( r1 < prob_r ) // Prob of reflection
      {
        DEBUG_LOG( "Launching recursive reflected ray." );
        threeVector direction = current_intersect.getObject()->getMaterial()->sampleReflection( current_intersect ).norm();
        DEBUG_STREAM << "NORMAL = " << current_intersect.getSurfaceNormal() << "  --  Direction leaving intersect = " << direction;
        beam incoming = roulette_factor*this->_pathTrace( current_intersect.getPoint(), direction, depth + 1 );

        DEBUG_STREAM << "Returned recursive ray at object: " << current_intersect.getObject();
        currentBeam += current_intersect.getObject()->getMaterial()->BRDF( -direction, incoming, current_intersect );
      }
      else if ( r1 < ( prob_r + prob_t ) )
      {
        DEBUG_LOG( "Launching recursive transmitted ray." );
        threeVector direction = current_intersect.getObject()->getMaterial()->sampleTransmission( current_intersect ).norm();
        beam incoming = roulette_factor*this->_pathTrace( current_intersect.getPoint(), direction, depth + 1 );

        currentBeam += current_intersect.getObject()->getMaterial()->BTDF( -direction, incoming, current_intersect );
      }
      else
      {
        DEBUG_LOG( "Path absorbed. Closing recursion" );
        return beam( 0.0, 0.0, 0.0 );
      }
    }
    else
    {
      DEBUG_LOG( "End of path reached closing recursion." );
    }

//    // Standard path tracing - just sample the emission from the vertex and return.
//     currentBeam += current_intersect.getObject()->getMaterial()->getEmission( current_intersect.getSurfaceMap() );


    // Add light from other light sources.
    DEBUG_STREAM << "Tracing Lights to object: " << current_intersect.getObject();
    currentBeam += roulette_factor*sampleAllLights( current_intersect );

    DEBUG_STREAM << "Returning Current Beam." << currentBeam.red() << ", " << currentBeam.green() << ", " << currentBeam.blue();
    return currentBeam;
  }


  beam tracer_pathtracer::traceRay( point start, threeVector dir )
  {
    return this->_pathTrace( start, dir ); // Starts the recursive function
  }

}


