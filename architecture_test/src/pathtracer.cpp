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


  spectrum tracer_pathtracer::_pathTrace( point start, threeVector dir, unsigned int depth )
  {
    line the_ray( start, dir );
    double kill_factor = ( depth < 3 ? 0.0 : _killProb );
    double roulette_factor = 1.0 / ( 1.0 - kill_factor );

    interaction inter;
    try
    {
      inter = this->getInteraction( the_ray );
    }
    catch( stdexts::exception& e )
    {
      std::stringstream ss;
      ss << "Path Tracer Depth = " << depth;
      EX_LOG( e, ss.str() );
      std::cerr << ELUCIDATE( e );
      return spectrum( 0.0, 0.0, 0.0 );
    }

    if ( inter.getObject() == nullptr )
    {
      return spectrum( 0.0, 0.0, 0.0 );
    }


    DEBUG_STREAM << "Interaction: " << inter.getObject() << " -- " << inter.getDistance() << " -- " << inter.getPoint().getPosition() << " | " << roulette_factor;


    spectrum currentBeam( 0.0, 0.0, 0.0 );

    if ( random::uniformDouble() > _killProb )
    {
      double r1 = random::uniformDouble();

      double prob_r = inter.getObject()->getMaterial()->getReflectionProb( inter );
      double prob_t = inter.getObject()->getMaterial()->getTransmissionProb( inter );

      DEBUG_STREAM <<  "Checking for recursive rays. Rand = " << r1 << ". Prob R = " << prob_r << ". Prob T = " << prob_t;


      if ( r1 < prob_r ) // Prob of reflection
      {
        DEBUG_LOG( "Launching recursive reflected ray." );
        threeVector direction = inter.getObject()->getMaterial()->sampleReflection( inter ).norm();

        spectrum incoming = this->_pathTrace( inter.getPoint(), direction, depth + 1 );
        DEBUG_STREAM << "Returned recursive ray at object: " << inter.getObject();

        double attenuation = direction * inter.getSurfaceNormal();

        // Normalised by the volume of the sample space : roulette * sample space * BRDF * Li * cos_theta
        currentBeam = roulette_factor * 2.0*PI * inter.getObject()->getMaterial()->BRDF( -direction, inter ) * incoming * attenuation;
      }
      else if ( r1 < ( prob_r + prob_t ) )
      {
        DEBUG_LOG( "Launching recursive transmitted ray." );
        threeVector direction = inter.getObject()->getMaterial()->sampleTransmission( inter ).norm();

        spectrum incoming = this->_pathTrace( inter.getPoint(), direction, depth + 1 );
        DEBUG_STREAM << "Returned recursive ray at object: " << inter.getObject();

        double attenuation = direction * inter.getSurfaceNormal();

        currentBeam = roulette_factor * 2.0*PI * inter.getObject()->getMaterial()->BTDF( -direction, inter ) * incoming * attenuation;
      }
      else
      {
        DEBUG_LOG( "Path absorbed. Closing recursion" );
        currentBeam = spectrum( 0.0, 0.0, 0.0 );
      }
    }
    else
    {
      DEBUG_LOG( "End of path reached closing recursion." );
      return spectrum( 0.0, 0.0, 0.0 );
    }


    // Standard path tracing - just sample the emission from the vertex and return.
    spectrum emission = inter.getObject()->getMaterial()->getEmission( inter.getSurfaceMap() );
    DEBUG_STREAM << "Adding emission from current intersection : " << emission.red() << ", " << emission.green() << ", " << emission.blue();
    currentBeam += roulette_factor * emission;



//    // Add light from other light sources.
//    DEBUG_STREAM << "Tracing Lights to object: " << inter.getObject();
//    currentBeam += roulette_factor*sampleAllLights( inter );
//
//    if ( depth == 0 ) // If the first hit is a light, include its contribution manually
//    {
//      currentBeam += inter.getObject()->getMaterial()->getEmission( inter.getSurfaceMap() );
//    }
//

    DEBUG_STREAM << "Returning Current Beam." << currentBeam.red() << ", " << currentBeam.green() << ", " << currentBeam.blue();
    return currentBeam;
  }


  spectrum tracer_pathtracer::traceRay( point start, threeVector dir )
  {
    DEBUG_LOG( "--  STARTING PATH TRACE  --" );
    return this->_pathTrace( start, dir ); // Starts the recursive function
  }

}


