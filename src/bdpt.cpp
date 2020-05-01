//#define LOGTASTIC_DEBUG_OFF

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
    _maxDepth( -1 ),
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


  void tracer_bdpt::_buildPath( path& the_path, line the_ray, spectrum throughput )
  {
    double kill_factor = _killProb;
    double roulette_factor = 1.0 / ( 1.0 - kill_factor );

    unsigned int vertex_count = 0;
    double r = 1.0;

    try
    {
      while( (r > kill_factor) && (vertex_count < _maxDepth) )
      {
        interaction inter = this->getInteraction( the_ray );
        if ( inter.getObject() == nullptr )
        {
          DEBUG_LOG( "No object intersected" );
          return;
        }
        DEBUG_STREAM << "Interaction: " << inter.getObject() << " -- " << inter.getDistance() << " -- " << inter.getPoint().getPosition() << " | " << roulette_factor;



        // Recursion bit...

        DEBUG_LOG( "Checking for secondary rays." );

        double r1 = random::uniformDouble();

        double prob_r = inter.getObject()->getMaterial()->getReflectionProb( inter );
        double prob_t = inter.getObject()->getMaterial()->getTransmissionProb( inter );

        if ( r1 < prob_r ) // Prob of reflection
        {
          DEBUG_LOG( "Reflection." );
          threeVector direction = inter.getObject()->getMaterial()->sampleReflection( inter ).norm();

          double attenuation = direction * inter.getSurfaceNormal();

          // Roulette * sample space volume * BRDF * cos_theta
          spectrum colour = inter.getObject()->getMaterial()->getColour( inter.getSurfaceMap() );
          throughput = 2.0*PI * inter.getObject()->getMaterial()->BRDF( -direction, inter ) * attenuation * colour;

          the_ray = line( inter.getPoint(), direction );
          DEBUG_STREAM << "New ray: " << inter.getPoint() << " -- " << direction;
        }
        else if ( r1 < ( prob_r + prob_t ) ) // Prob of transmission
        {
          DEBUG_LOG( "Transmission." );
          threeVector direction = inter.getObject()->getMaterial()->sampleTransmission( inter ).norm();

          double attenuation = direction * inter.getSurfaceNormal();

          // Roulette * sample space volume * BRDF * cos_theta
          spectrum colour = inter.getObject()->getMaterial()->getColour( inter.getSurfaceMap() );
          throughput = 2.0*PI * inter.getObject()->getMaterial()->BTDF( -direction, inter ) * attenuation * colour;

          the_ray = line( inter.getPoint(), direction );
          DEBUG_STREAM << "New ray: " << inter.getPoint() << " -- " << direction;
        }
        else
        {
          DEBUG_LOG( "Path absorbed." );
//          spectrum colour = inter.getObject()->getMaterial()->getColour( inter.getSurfaceMap() );
//          throughput = inter.getObject()->getMaterial()->getEmission( inter.getSurfaceMap() ) * colour;
          return;
        }


        the_path.push_back( pathvertex( inter, throughput, roulette_factor ) );



        vertex_count += 1;
        r = random::uniformDouble();
        DEBUG_STREAM << "Rand r = " << r << ", " << kill_factor << " : " << (r > kill_factor);
      }

      DEBUG_LOG( "Path built." );
      DEBUG_STREAM << "Last Vertex No.: " << vertex_count;
    }
    catch( stdexts::exception& e )
    {
      std::stringstream ss;
      ss << "Build path vertex No. = " << vertex_count;
      EX_LOG( e, ss.str() );
      RETHROW( e );
    }
  }


  spectrum tracer_bdpt::traceRay( point start, threeVector dir )
  {
    DEBUG_LOG( " -- BI DIRECTIONAL PATH TRACING -- " );
    line camera_ray( start, dir );

    _lightPath = path(); // Reset the paths
    _cameraPath = path();

    try // Load the camera path
    {
      this->_buildPath( _cameraPath, camera_ray, spectrum( 1.0, 1.0, 1.0 ) );
    }
    catch( stdexts::exception& e )
    {
      std::stringstream ss;
      EX_LOG( e, std::string( "Building camera path" ) );
      std::cerr << ELUCIDATE( e );
      return spectrum( 0.0, 0.0, 0.0 );
    }

    if ( _cameraPath.empty() ) // Camera staring into the void.
    {
      DEBUG_LOG( "Camera path is empty" );
      return spectrum( 0.0, 0.0, 0.0 );
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
//      return spectrum( 0.0, 0.0, 0.0 );
//    }


      spectrum result = this->directLighting( _cameraPath.begin()->getInteraction() );

      path::reverse_iterator end = _cameraPath.rend();
      for ( path::reverse_iterator it = _cameraPath.rbegin(); it != end; ++it )
      {
//        spectrum emitted = this->directLighting( it->getInteraction() );
        spectrum emitted = it->getInteraction().getObject()->getMaterial()->getExitantRadiance( it->getInteraction() );
        result = it->getWeight() * (it->getThroughput() * result + emitted);
      }

      DEBUG_STREAM <<  " -- END OF PATH TRACING : " << result.red() << ", " << result.green() << ", " << result.blue();
      return result;
  }

}


