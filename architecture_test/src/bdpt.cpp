//#define __DEBUG_OFF__

#include "S3D_bdpt.h"

#include "S3D_defs.h"
#include "S3D_line.h"
#include "S3D_manager.h"
#include "S3D_light_base.h"
#include "S3D_interaction.h"
#include "S3D_random.h"

#include "logtastic.h"
#include "stdexts.h"

#include <sstream>


namespace S3D
{

  tracer_bdpt::tracer_bdpt() :
    _killProb( 0.7 ),
    _lightPath(),
    _cameraPath()
  {
  }


  tracer_bdpt::~tracer_bdpt()
  {
  }


  void tracer_bdpt::setKillProb( double k )
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


  void tracer_bdpt::_additionalSetup()
  {
    if ( this->getTotalLightArea() < epsilon )
    {
      FAILURE_LOG( "tracer_bdpt::_additionalSetup(): No light-emitting area found in the scene. Cannot use BDPT." );
      stdexts::exception ex( "No light", "No light-emitting area found in the scene. Cannot use BDPT." );
      EX_CREATE( ex );
      THROW( ex );
    }
  }


//  const object_base* tracer_bdpt::_chooseLight() const
//  {
//    double r = random::uniformDouble() * _totalLightArea;
//    double currentSum = 0.0;
//    const object_base* light_obj;
//    LightListT lights = _getLights();
//    LightListT::const_iterator lights_end = lights.end();
//
//    for ( LightListT::const_iterator it = lights.begin(); it != lights_end; ++it )
//    {
//      light_obj = (*it);
//      currentSum += light_obj->getSurfaceArea();
//      if ( r < currentSum )
//        return light_obj;
//    }
//    ERROR_LOG( "tracer_bdpt::_chooseLight(): Random number outside range - should not have reached this point... What to do?" );
//    return nullptr;
//  }


  void tracer_bdpt::_buildPath( path& the_path, line the_ray, beam the_beam )
  {
    double kill_factor = 1.0 - _killProb;

    double current_kill = 1.0;
    unsigned int vertex_count = 0;

    try
    {
      do
      {
        interaction current_intersect = this->getInteraction( the_ray );
        if ( current_intersect.getObject() == nullptr )
          return;



        // Recursion bit...

        DEBUG_LOG( "Checking for secondary rays." );

        double r1 = random::uniformDouble();

        double prob_r = current_intersect.getObject()->getMaterial()->getReflectionProb( current_intersect );
        double prob_t = current_intersect.getObject()->getMaterial()->getTransmissionProb( current_intersect );

        if ( r1 < prob_r ) // Prob of reflection
        {
          DEBUG_LOG( "Reflection." );
          threeVector direction = current_intersect.getObject()->getMaterial()->sampleReflection( current_intersect ).norm();

          the_beam = (1.0/kill_factor) * current_intersect.getObject()->getMaterial()->BRDF( -direction, the_beam, current_intersect );

          the_ray = line( current_intersect.getPoint(), direction );
          DEBUG_STREAM << "New ray: " << current_intersect.getPoint() << " -- " << direction;

        }
        else if ( r1 < ( prob_r + prob_t ) ) // Prob of transmission
        {
          DEBUG_LOG( "Transmission." );
          threeVector direction = current_intersect.getObject()->getMaterial()->sampleTransmission( current_intersect ).norm();

          the_beam = (1.0/kill_factor) * current_intersect.getObject()->getMaterial()->BTDF( -direction, the_beam, current_intersect );

          the_ray = line( current_intersect.getPoint(), direction );
          DEBUG_STREAM << "New ray: " << current_intersect.getPoint() << " -- " << direction;
        }
        else
        {
          DEBUG_LOG( "Path absorbed." );
          the_beam = current_intersect.getObject()->getMaterial()->getEmission( current_intersect.getSurfaceMap() );
          return;
        }


        the_path.push_back( pathvertex( current_intersect, 1.0/current_kill, the_beam ) );



        current_kill *= kill_factor;
        vertex_count += 1;
      }
      while ( random::uniformDouble() < current_kill );
      DEBUG_LOG( "Path built." );
    }
    catch( stdexts::exception& e )
    {
      std::stringstream ss;
      ss << "Build path vertex No. = " << vertex_count;
      EX_LOG( e, ss.str() );
      RETHROW( e );
    }
  }


  beam tracer_bdpt::traceRay( point start, threeVector dir )
  {
    line camera_ray( start, dir );

    _lightPath = path(); // Reset the paths
    _cameraPath = path();

    try // Load the camera path
    {
      this->_buildPath( _cameraPath, camera_ray, beam( 1.0, 1.0, 1.0 ) );
    }
    catch( stdexts::exception& e )
    {
      std::stringstream ss;
      EX_LOG( e, std::string( "Building camera path" ) );
      std::cerr << ELUCIDATE( e );
      return beam( 0.0, 0.0, 0.0 );
    }

    if ( _cameraPath.empty() ) // Camera staring into the void.
    {
      return beam( 0.0, 0.0, 0.0 );
    }

    // Leave this out for testing & comparisons with pathtracing
//    try // Load the light path
//    {
//      do
//      {
//        const object_base* the_light = this->_chooseLight();
//        this->_buildPath( _lightPath, the_light.sampleEmission() );
//      }
//      while ( _lightPath.isEmpty() ); // Require at least one vertex
//    }
//    catch( stdexts::exception& e )
//    {
//      std::stringstream ss;
//      EX_LOG( e, std::string( "Building light path" );
//      std::cerr << ELUCIDATE( e );
//      return beam( 0.0, 0.0, 0.0 );
//    }

      const object_base* the_light = this->_chooseLight();

      pathvertex last_vertex = (*_cameraPath.rbegin());
      interaction last_interaction = last_vertex.getInteraction();

      beam light_beam = this->sampleLight( the_light, last_interaction );

      return light_beam * last_vertex.getBeam();
  }

}


