#define LOGTASTIC_DEBUG_OFF

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
    double kill_factor = ( depth <= 5 ? 0.0 : _killProb );
    double roulette_factor = 1.0 / ( 1.0 - kill_factor );
    spectrum currentBeam( 0.0 );



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
      return currentBeam;
    }

    DEBUG_STREAM << "Interaction: " << inter.getObject() << " -- " << inter.getDistance() << " -- " << inter.getPoint().getPosition() << " | " << roulette_factor;



    currentBeam = roulette_factor * inter.getObject()->getMaterial()->getExitantRadiance( inter );
    DEBUG_STREAM << " EMITTANCE: Roulette = " << roulette_factor << ", Lo = " << inter.getObject()->getMaterial()->getExitantRadiance( inter ).red();


    if ( (random::uniformDouble() < kill_factor) )
    {
      DEBUG_LOG( "End of path reached closing recursion." );
      return currentBeam;
    }

    if ( depth >= _maxDepth )
    {
      WARN_LOG( "Path reached maximum depth" );
      return currentBeam;
    }



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

      // Normalised by the volume of the sample space : sample space * BRDF * Li * cos_theta * albedo
      spectrum colour = inter.getObject()->getMaterial()->getColour( inter.getSurfaceMap() );
      currentBeam += roulette_factor * 2.0*PI * inter.getObject()->getMaterial()->BRDF( -direction, inter ) * incoming * attenuation * colour;

      DEBUG_STREAM << " REFLECTION: Roulette = " << roulette_factor << ", BRDF = " << inter.getObject()->getMaterial()->BRDF( -direction, inter ) << ", Li = " << incoming.red() << ", cos_theta = " << attenuation << ", albedo = " << colour.red();
    }
    else if ( r1 < ( prob_r + prob_t ) )
    {
      DEBUG_LOG( "Launching recursive transmitted ray." );
      threeVector direction = inter.getObject()->getMaterial()->sampleTransmission( inter ).norm();

      spectrum incoming = this->_pathTrace( inter.getPoint(), direction, depth + 1 );
      DEBUG_STREAM << "Returned recursive ray at object: " << inter.getObject();

      double attenuation = direction * inter.getSurfaceNormal();
      spectrum colour = inter.getObject()->getMaterial()->getColour( inter.getSurfaceMap() );
      currentBeam += roulette_factor * 2.0*PI * inter.getObject()->getMaterial()->BTDF( -direction, inter ) * incoming * attenuation * colour;

      DEBUG_STREAM << " TRANSMISSION: Roulette = " << roulette_factor << ", BTDF = " << inter.getObject()->getMaterial()->BTDF( -direction, inter ) << ", Li = " << incoming.red() << ", cos_theta = " << attenuation << ", albedo = " << colour.red();
    }
    else
    {
      DEBUG_LOG( "Path absorbed. Closing recursion" );
    }




    // Standard path tracing - just sample the emission from the vertex and return.
//    currentBeam += roulette_factor * inter.getObject()->getMaterial()->getExitantRadiance( inter );
//    DEBUG_STREAM << " EMITTANCE: Roulette = " << roulette_factor << ", Lo = " << inter.getObject()->getMaterial()->getExitantRadiance( inter ).red();




//    // Add light from other light sources.
//    DEBUG_STREAM << "Tracing Lights to object: " << inter.getObject();
//    currentBeam += directLighting( inter );

//    const object_base* light_obj = this->_chooseLight();
//    DEBUG_STREAM << "  Current Light: " << light_obj;
//
//    surfacemap light_point = light_obj->sampleSurface();
//    unsigned int numLightSamples = this->_getNumLightSamples();
//
//    for ( unsigned int n = 0; n < numLightSamples; ++n )
//    {
//      surfacemap light_point = light_obj->sampleSurface();
//
//      if ( isVisible( light_point.getPosition(), inter.getPoint(), inter.getSurfaceNormal() ) )
//      {
//        threeVector lightDir = (inter.getPoint() - light_point.getPosition());
//        spectrum light_emission = light_obj->getMaterial()->getEmission( light_point );
//
//
//        // samplespace volume * BRDF * Li * Cos_theta
//        currentBeam += ( roulette_factor / (double)numLightSamples ) * inter.getObject()->getMaterial()->BRDF( lightDir.norm(), inter ) * light_emission * ( -inter.getSurfaceNormal() * lightDir );
//      }
//    }




//    if ( depth == 0 ) // If the first hit is a light, include its contribution manually
//    {
//      spectrum colour = inter.getObject()->getMaterial()->getColour( inter.getSurfaceMap() );
//      currentBeam += inter.getObject()->getMaterial()->getEmission( inter.getSurfaceMap() ) * colour;
//    }


    DEBUG_STREAM << "Returning Current Beam." << currentBeam.red() << ", " << currentBeam.green() << ", " << currentBeam.blue();
    return currentBeam;
  }


  spectrum tracer_pathtracer::traceRay( point start, threeVector dir )
  {
    DEBUG_LOG( "--  STARTING PATH TRACE  --" );
    return this->_pathTrace( start, dir ); // Starts the recursive function
  }

}


