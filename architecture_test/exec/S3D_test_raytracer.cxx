
#include "S3D_manager.h"
#include "S3D_version.h"
#include "S3D_primitives.h"
#include "S3D_materials.h"
#include "S3D_lights.h"
#include "S3D_cameras.h"
#include "S3D_recursiveraytracer.h"
#include "S3D_pathtracer.h"
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
  logtastic::setPrintToScreenLimit( logtastic::error );
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

  tracer_recursive rt;
  rt.setup();

  ASSERT_EQUAL( rt.isVisible( p1, p2, (p1-p2) ), false );
  ASSERT_EQUAL( rt.isVisible( p3, p2, (p3-p2) ), true );
  ASSERT_EQUAL( rt.isVisible( p4, p5, (p4-p5) ), false );

//  beam result = rt.traceRay( p4, p5 - p4 );
  rt.traceRay( p4, p5 - p4 );


////////////////////////////////////////////////////////////////////////////////////////////////////
  // Energy conservation and consistency.

  SECTION( "Bias and Consistency" );

  material_base* glowing = new material_glowing( S3D::colour( 0.3, 0.3, 0.3 ), 1.0 );
  object_base* glowing_sphere = new sphere( glowing, 2.0 );
  glowing_sphere->setPosition( point( 10.0, 0.0, 0.0 ) );
  man->addObject( glowing_sphere );

  tracer_pathtracer pt;
  pt.setLightSampleRate( 0.1 );
  pt.setup();

  beam test1( 0.0, 0.0, 0.0 );
  beam test2( 0.0, 0.0, 0.0 );
  beam test3( 0.0, 0.0, 0.0 );
  beam test4( 0.0, 0.0, 0.0 );
  beam test5( 0.0, 0.0, 0.0 );

  const unsigned int N = 10000;

  pt.setKillProb( 0.01 );
  for ( unsigned int i = 0; i < 100; ++i )
  {
    test1 += (1.0/100) * pt.traceRay( point( 10.0, 0.0, 0.0 ), unit_threeVector_z );
  }

  ASSERT_APPROX_EQUAL( test1.red(), test1.green() );
  ASSERT_APPROX_EQUAL( test1.green(), test1.blue() );

  pt.setKillProb( 0.5 );
  for ( unsigned int i = 0; i < N; ++i )
  {
    test2 += (1.0/N) * pt.traceRay( point( 10.0, 0.0, 0.0 ), unit_threeVector_z );
  }

  ASSERT_APPROX_EQUAL( test2.red(), test2.green() );
  ASSERT_APPROX_EQUAL( test2.green(), test2.blue() );

  pt.setKillProb( 0.1 );
  for ( unsigned int i = 0; i < N; ++i )
  {
    test3 += (1.0/N) * pt.traceRay( point( 10.0, 0.0, 0.0 ), unit_threeVector_z );
  }

  ASSERT_APPROX_EQUAL( test3.red(), test3.green() );
  ASSERT_APPROX_EQUAL( test3.green(), test3.blue() );

  pt.setKillProb( 0.5 );
  for ( unsigned int i = 0; i < N; ++i )
  {
    test4 += (1.0/N) * pt.traceRay( point( 10.0, 0.0, 0.0 ), unit_threeVector_z );
  }

  ASSERT_APPROX_EQUAL( test4.red(), test4.green() );
  ASSERT_APPROX_EQUAL( test4.green(), test4.blue() );

  pt.setKillProb( 0.01 );
  for ( unsigned int i = 0; i < N; ++i )
  {
    test5 += (1.0/N) * pt.traceRay( point( 10.0, 0.0, 0.0 ), unit_threeVector_z );
  }

  ASSERT_APPROX_EQUAL( test5.red(), test5.green() );
  ASSERT_APPROX_EQUAL( test5.green(), test5.blue() );

  ASSERT_GREATERTHAN( test1.red(), 0.001 );
  ASSERT_GREATERTHAN( test2.red(), 0.001 );
  ASSERT_GREATERTHAN( test3.red(), 0.001 );
  ASSERT_GREATERTHAN( test4.red(), 0.001 );
  ASSERT_GREATERTHAN( test5.red(), 0.001 );

  ASSERT_LESSTHAN( std::fabs( test1.red() - test2.red() )/ test1.red(), 0.05 );
  ASSERT_LESSTHAN( std::fabs( test2.red() - test3.red() )/ test2.red(), 0.05 );
  ASSERT_LESSTHAN( std::fabs( test3.red() - test4.red() )/ test3.red(), 0.02 );
  ASSERT_LESSTHAN( std::fabs( test4.red() - test5.red() )/ test4.red(), 0.02 );

  INFO_STREAM << "Test results:";
  INFO_STREAM << "Test1: Red = " << test1.red() << ", " << test1.green() << ", " << test1.blue();
  INFO_STREAM << "Test2: Red = " << test2.red() << ", " << test2.green() << ", " << test2.blue();
  INFO_STREAM << "Test3: Red = " << test3.red() << ", " << test3.green() << ", " << test3.blue();
  INFO_STREAM << "Test4: Red = " << test4.red() << ", " << test4.green() << ", " << test4.blue();
  INFO_STREAM << "Test5: Red = " << test5.red() << ", " << test5.green() << ", " << test5.blue();

  
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


