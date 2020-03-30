
#include "S3D_manager.h"
#include "S3D_version.h"
#include "S3D_primitives.h"
#include "S3D_materials.h"
#include "S3D_lights.h"
#include "S3D_cameras.h"
#include "S3D_defs.h"

#include "logtastic.h"
#include "testass.h"

#include <iostream>
#include <iomanip>
#include <string>


using namespace S3D;


int main( int, char** )
{
  testass::control::init( "S3D", "Testing The Ray Tracer" );
  testass::control::get()->setVerbosity( testass::control::verb_short );

  logtastic::setLogFileDirectory( "./test_data/" );
  logtastic::addLogFile( "./raytracer_test.log" );
  logtastic::init( "Testing S3D Basics", S3D_VERSION_NUMBER );

  S3D::manager::createInstance();

  S3D::manager* man = S3D::manager::getInstance();


  INFO_LOG( "Making basic material" );
  S3D::material_base* mat = (S3D::material_base*) new S3D::material_simple( S3D::colour( 1.0, 0.0, 0.0 ) );
  man->addMaterial( "simple", mat );
  S3D::material_base* world_mat = (S3D::material_base*) new S3D::material_simple( S3D::colour( 0.0, 0.3, 0.8 ) );
  man->addMaterial( "room", world_mat );


  INFO_LOG( "Making world sphere" );
  S3D::object_base* world = (S3D::object_base*) new S3D::sphere( mat, 100.0 );
  man->setWorld( world, 0 );


////////////////////////////////////////////////////////////////////////////////////////////////////


  SECTION( "Visibility" );

  point p1( -5.0, 0.0, 0.0 );
  point p2( 5.0, 0.0, 0.0 );
  point p3( 5.0, 10.0, 0.0 );
  point p4( -5.0, 0.9, 0.0 );
  point p5( 5.0, 0.9, 0.0 );

  object_base* the_sphere = new sphere( man->getMaterial( "simple" ), 1.0 );
  the_sphere->setPosition( point( 0.0 ) );
  man->addObject( the_sphere, 0 );

  rayTracer rt;

  ASSERT_EQUAL( rt.isVisible( p1, p2 ), false );
  ASSERT_EQUAL( rt.isVisible( p3, p2 ), true );
  ASSERT_EQUAL( rt.isVisible( p4, p5 ), false );

//  beam result = rt.traceRay( p4, p5 - p4 );
  rt.traceRay( p4, p5 - p4 );

  
////////////////////////////////////////////////////////////////////////////////////////////////////


  if ( ! testass::control::summarize() )
  {
    testass::control::printReport( std::cout );
  }

  S3D::manager::killInstance();

  testass::control::kill();
  logtastic::stop();
  return 0;
}


