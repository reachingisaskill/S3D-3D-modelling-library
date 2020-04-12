
#include "S3D_manager.h"
#include "S3D_version.h"
#include "S3D_primitives.h"
#include "S3D_materials.h"
#include "S3D_lights.h"
#include "S3D_cameras.h"
#include "S3D_defs.h"
#include "S3D_random.h"
#include "S3D_convergence.h"

#include "logtastic.h"
#include "testass.h"

#include <iostream>
#include <iomanip>
#include <string>


int main( int, char** )
{
  testass::control::init( "S3D", "Basic Structures and Classes" );
  testass::control::get()->setVerbosity( testass::control::verb_short );

  logtastic::setLogFileDirectory( "./test_data/" );
  logtastic::addLogFile( "./basics_test.log" );
  logtastic::init( "Testing S3D Basics", S3D_VERSION_NUMBER );

  S3D::manager::createInstance();

  S3D::manager* man = S3D::manager::getInstance();


  INFO_LOG( "Making basic material" );
  S3D::material_base* mat = (S3D::material_base*) new S3D::material_simple( S3D::spectrum( 1.0, 0.0, 0.0 ) );
  man->addMaterial( "simple", mat );
  S3D::material_base* world_mat = (S3D::material_base*) new S3D::material_simple( S3D::spectrum( 0.0, 0.3, 0.8 ) );
  man->addMaterial( "room", world_mat );


  INFO_LOG( "Making world sphere" );
  S3D::object_base* world = (S3D::object_base*) new S3D::sphere( mat, 100.0 );
  man->setWorld( world, 1 );



////////////////////////////////////////////////////////////////////////////////////////////////////

  SECTION( "Rotation" );
  {

    S3D::rotation rot1( makeThreeVector( 0.0, 0.0, 1.0 ), S3D::degreesToRadians( 90 ) );
    S3D::rotation rot2( makeThreeVector( 1.0, 1.0, 0.0 ), S3D::degreesToRadians( 45 ) );
    S3D::rotation rot3 = -rot2;
    S3D::rotation rot4 = rot1 * rot2;
    threeVector vec1 = makeThreeVector( 0.0, 1.0, 0.0 );
    threeVector vec2 = makeThreeVector( 0.0, 10.0, 15.0 );
    threeVector vec3 = makeThreeVector( 1.0, 0.0, 0.0 );

    threeVector p1 = makeThreeVector( 0.0, 1.0, 0.0 );
    threeVector p2 = makeThreeVector( 0.0, 10.0, 15.0 );
    threeVector p3 = makeThreeVector( 1.0, 0.0, 0.0 );

    ASSERT_APPROX_EQUAL( rot1.getAxis().mod(), 1.0 );
    ASSERT_APPROX_EQUAL( rot2.getAxis().mod(), 1.0 );
    ASSERT_APPROX_EQUAL( rot3.getAxis().mod(), 1.0 );
    ASSERT_APPROX_EQUAL( rot4.getAxis().mod(), 1.0 );

    threeVector newVec = rot1.rotateVector( vec1 );
    ASSERT_APPROX_EQUAL( newVec[0], -1.0 );
    ASSERT_APPROX_EQUAL( newVec[1], 0.0 );
    ASSERT_APPROX_EQUAL( newVec[2], 0.0 );

    threeVector newPoint = rot1.rotateVector( p1 );
    ASSERT_EQUAL( newVec, newPoint );

    newVec = rot1.rotateVector( vec2 );
    ASSERT_APPROX_EQUAL( newVec[0], -10.0 );
    ASSERT_APPROX_EQUAL( newVec[1], 0.0 );
    ASSERT_APPROX_EQUAL( newVec[2], 15.0 );

    newPoint = rot1.rotateVector( p2 );
    ASSERT_EQUAL( newVec, newPoint );

    newVec = rot2.rotateVector( vec3 );
    ASSERT_APPROX_EQUAL( newVec[0], 0.853553391 ); // Used a calculator for this one!
    ASSERT_APPROX_EQUAL( newVec[1], 0.1464466094 );// No really I did!
    ASSERT_APPROX_EQUAL( newVec[2], -0.5 ); // Honest!

    newPoint = rot2.rotateVector( p3 );
    ASSERT_EQUAL( newVec, newPoint );

    newVec = rot3 * newVec;
    ASSERT_APPROX_EQUAL( newVec[0], 1.0 );
    ASSERT_APPROX_EQUAL( newVec[1], 0.0 );
    ASSERT_APPROX_EQUAL( newVec[2], 0.0 );

    ASSERT_APPROX_EQUAL( rot4.getAxis().mod(), 1.0 );

    newVec = ( rot1 * ( rot2 * vec2 ) );
    threeVector otherVec = rot4 * vec2;
    ASSERT_APPROX_EQUAL( newVec[0], otherVec[0] );
    ASSERT_APPROX_EQUAL( newVec[1], otherVec[1] );
    ASSERT_APPROX_EQUAL( newVec[2], otherVec[2] );

  }

////////////////////////////////////////////////////////////////////////////////////////////////////

  SECTION( "Point" );

  S3D::point p1( 0.0, 0.0, 0.0 );
  ASSERT_EQUAL( p1[0], 0.0 );
  ASSERT_EQUAL( p1[1], 0.0 );
  ASSERT_EQUAL( p1[2], 0.0 );

  S3D::point p2( makeThreeVector( 1.0, 2.0, 3.0 ) );
  ASSERT_EQUAL( p2[0], 1.0 );
  ASSERT_EQUAL( p2[1], 2.0 );
  ASSERT_EQUAL( p2[2], 3.0 );

  S3D::point p3( 15.7 );
  ASSERT_EQUAL( p3[0], 15.7 );
  ASSERT_EQUAL( p3[1], 15.7 );
  ASSERT_EQUAL( p3[2], 15.7 );

////////////////////////////////////////////////////////////////////////////////////////////////////

  SECTION( "Line" );

  S3D::line l1( S3D::point( 1.0, 2.0, 3.0 ), makeThreeVector( 1.0, 1.0, 10.0 ) );
  S3D::line l2( S3D::point( 1.0, 2.0, 4.0 ), makeThreeVector( 1.0, 1.0, 10.0 ) );
  ASSERT_EQUAL( l1.getStart(), S3D::point( 1.0, 2.0, 3.0 ) );
  ASSERT_EQUAL( l1.getDirection(), makeThreeVector( 1.0, 1.0, 10.0 ).norm() );
  ASSERT_EQUAL( l2.getStart(), S3D::point( 1.0, 2.0, 4.0 ) );
  ASSERT_EQUAL( l2.getDirection(), makeThreeVector( 1.0, 1.0, 10.0 ).norm() );
  ASSERT_APPROX_EQUAL( l1.getDirection().mod(), 1.0 );
  ASSERT_APPROX_EQUAL( l2.getDirection().mod(), 1.0 );
  ASSERT_EQUAL( l1.getDirection(), l2.getDirection() );
  ASSERT_APPROX_EQUAL( l1.distance( l2 ), 0.1400280084 );

////////////////////////////////////////////////////////////////////////////////////////////////////

  SECTION( "Surface" );

  threeVector test = makeThreeVector( 1.0, -1.0, std::sqrt(2.0) ).norm();
  S3D::surface s0( S3D::point( 0.0 ), S3D::rotation());
  S3D::surface s1( S3D::point( 0.0 ), S3D::rotation( makeThreeVector( 1.0, 1.0, 0.0 ), 0.25*S3D::PI ) );
  S3D::surface s2( S3D::point( 1.0, 10.0, 3.0 ), S3D::rotation( makeThreeVector( 0.0, 1.0, 0.0 ), 0.5*S3D::PI ) );

  ASSERT_EQUAL( s0.getNormal(), S3D::unit_threeVector_z );
  ASSERT_EQUAL( s0.inFront( S3D::point( 0.0, 0.0, 1.0 ) ), true );
  ASSERT_EQUAL( s0.inFront( S3D::point( 0.0, 0.0, -1.0 ) ), false );
  ASSERT_EQUAL( s1.getPosition().getPosition(), makeThreeVector( 0.0, 0.0, 0.0 ) );
  ASSERT_APPROX_EQUAL( s1.getNormal()[0], test[0] );
  ASSERT_APPROX_EQUAL( s1.getNormal()[1], test[1] );
  ASSERT_APPROX_EQUAL( s1.getNormal()[2], test[2] );
  ASSERT_EQUAL( s1.inFront( S3D::point( 1.0, 2.0, -3.0 ) ), false );
  ASSERT_EQUAL( s1.crosses( l2 ), false );
  ASSERT_EQUAL( s2.inFront( S3D::point( 10.0, 0.0, 0.0 ) ), true );
  ASSERT_EQUAL( s2.inFront( S3D::point( 1.0-S3D::epsilon, 0.0, 0.0 ) ), false );

  S3D::line l3( S3D::point( 0.0, 0.0, 1.0 ), makeThreeVector( 0.0, 0.0, -1.0 ) );
  ASSERT_TRUE( s1.crosses( l3 ) );

  S3D::line l4( S3D::point( 0.0, 0.0, 1.0 ), makeThreeVector( 0.0, 0.0, 1.0 ) );
  ASSERT_FALSE( s1.crosses( l4 ) );

  S3D::point inter1 = s1.intersect( l3 );
  ASSERT_APPROX_EQUAL( inter1[0], 0.0 );
  ASSERT_APPROX_EQUAL( inter1[1], 0.0 );
  ASSERT_APPROX_EQUAL( inter1[2], 0.0 );


////////////////////////////////////////////////////////////////////////////////////////////////////

  SECTION( "Random Samples ");

  threeVector direction = S3D::unit_threeVector_x;

  unsigned int counter = 0;
  for ( unsigned int i = 0; i < 1000000; ++i ) // Sample the directions.
  {
    threeVector test = S3D::random::uniformHemisphere( direction );

    if ( ( test * direction ) < 0.0 ) // if going backwards
      counter += 1;
  }
  ASSERT_EQUAL( counter, (unsigned int)0 );


  direction = S3D::rotation( S3D::unit_threeVector_z, 0.325 ).rotateVector( direction );

  counter = 0;
  for ( unsigned int i = 0; i < 1000000; ++i ) // Sample the directions.
  {
    threeVector test = S3D::random::uniformHemisphere( direction );

    if ( ( test * direction ) < 0.0 ) // if going backwards
      counter += 1;
  }
  ASSERT_EQUAL( counter, (unsigned int)0 );

  S3D::random::halton h2( 2 );
  S3D::random::halton h3( 3 );

  ASSERT_APPROX_EQUAL( h2.sample(), 1.0/2.0 );
  ASSERT_APPROX_EQUAL( h2.sample(), 1.0/4.0 );
  ASSERT_APPROX_EQUAL( h2.sample(), 3.0/4.0 );
  ASSERT_APPROX_EQUAL( h2.sample(), 1.0/8.0 );
  ASSERT_APPROX_EQUAL( h2.sample(), 5.0/8.0 );
  ASSERT_APPROX_EQUAL( h2.sample(), 3.0/8.0 );
  ASSERT_APPROX_EQUAL( h2.sample(), 7.0/8.0 );
  ASSERT_APPROX_EQUAL( h2.sample(), 1.0/16.0 );

  ASSERT_APPROX_EQUAL( h3.sample(), 1.0/3.0 );
  ASSERT_APPROX_EQUAL( h3.sample(), 2.0/3.0 );
  ASSERT_APPROX_EQUAL( h3.sample(), 1.0/9.0 );
  ASSERT_APPROX_EQUAL( h3.sample(), 4.0/9.0 );
  ASSERT_APPROX_EQUAL( h3.sample(), 7.0/9.0 );
  ASSERT_APPROX_EQUAL( h3.sample(), 2.0/9.0 );
  ASSERT_APPROX_EQUAL( h3.sample(), 5.0/9.0 );
  ASSERT_APPROX_EQUAL( h3.sample(), 8.0/9.0 );


////////////////////////////////////////////////////////////////////////////////////////////////////


  SECTION( "Utility Functionality" );
  {

    S3D::convergence_counter c_counter( 5 );
    S3D::spectrum test_spectrum( 0.0, 0.0, 0.0 );

    ASSERT_TRUE( c_counter( test_spectrum ) );
    ASSERT_TRUE( c_counter( test_spectrum ) );
    ASSERT_TRUE( c_counter( test_spectrum ) );
    ASSERT_TRUE( c_counter( test_spectrum ) );
    ASSERT_FALSE( c_counter( test_spectrum ) );

    S3D::convergence_error c_variance( 0.01 );

    ASSERT_TRUE( c_variance( test_spectrum ) );
    ASSERT_TRUE( c_variance( test_spectrum ) );
    ASSERT_TRUE( c_variance( test_spectrum ) );
    ASSERT_TRUE( c_variance( test_spectrum ) );
    ASSERT_FALSE( c_variance( test_spectrum ) );
    INFO_STREAM << "C_Variance: " << c_variance.getMeanIntensity() << ", " << c_variance.getFractionalVariance() << ", " << c_variance.getVariance();


    c_variance.clear();

    test_spectrum = S3D::spectrum( 1.0, 1.0, 1.0 );

    c_variance( test_spectrum );
    c_variance( test_spectrum );

    test_spectrum = S3D::spectrum( 5.0, 5.0, 5.0 );
    c_variance( test_spectrum );
    c_variance( test_spectrum );

    ASSERT_TRUE( c_variance( test_spectrum ) );

    ASSERT_APPROX_EQUAL( c_variance.getMeanIntensity(), 17.0/5.0 );
    ASSERT_APPROX_EQUAL( c_variance.getVariance(), 3.84 );

    S3D::spectrum mean_spec = c_variance.getMean();
    ASSERT_APPROX_EQUAL( mean_spec.red(), 17.0/5.0 );
    ASSERT_APPROX_EQUAL( mean_spec.green(), 17.0/5.0 );
    ASSERT_APPROX_EQUAL( mean_spec.blue(), 17.0/5.0 );


  }

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

