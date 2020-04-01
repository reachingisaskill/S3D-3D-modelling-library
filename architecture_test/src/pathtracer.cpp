//#define __DEBUG_OFF__

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


  beam tracer_pathtracer::_pathTrace( point start, threeVector dir, unsigned int depth ) const
  {
    line the_ray( start, dir );
    double kill_factor = _killProb;
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
//      double prob_t = 1.0 - prob_r;
      double attenuation = current_intersect.getAttenuation();

      if ( r1 < prob_r ) // Prob of reflection
      {
        DEBUG_LOG( "Launching recursive reflected ray." );
        currentBeam += roulette_factor*attenuation*this->_pathTrace( current_intersect.getPoint(), current_intersect.getObject()->getMaterial()->sampleReflection( current_intersect ), depth + 1 );
      }
      else // otherwise transmission
      {
        DEBUG_LOG( "Launching recursive transmitted ray." );
        currentBeam += roulette_factor*attenuation*this->_pathTrace( current_intersect.getPoint(), current_intersect.getObject()->getMaterial()->sampleTransmission( current_intersect ), depth + 1 );
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
        currentBeam += (*light_it)->sampleRays( current_intersect, this );
        DEBUG_STREAM << " Current Beam: " << currentBeam.red() << ", " << currentBeam.green() << ", " << currentBeam.blue();
        ++light_it;
      }
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


  beam tracer_pathtracer::traceRay( point start, threeVector dir ) const
  {
    return this->_pathTrace( start, dir ); // Starts the recursive function
  }

}


