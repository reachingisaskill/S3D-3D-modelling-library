

#define S3D_TEST_FUNCTIONALITY

#include <iostream>
#include <iomanip>
#include <string>

#include "testass.h"

#include "S3D.h"


int main( int, char** )
{
  testass::control::init( "S3D", "Basic Structures and Classes" );
  testass::control::get()->setVerbosity( testass::control::verb_short );

  S3D::manager::createInstance();
  S3D::manager::getInstance()->setWorld( new S3D::box( 200, 200, 200, threeVector( 0.0 ), S3D::rotation() ) );

////////////////////////////////////////////////////////////////////////////////////////////////////

  SECTION( "Colour" );
  
  S3D::colour col1;
  S3D::colour col2( 10.0, 20.0, 30.0, 0.3 );
  ASSERT_EQUAL( col2.getRed(), 10.0 );
  ASSERT_EQUAL( col2.getGreen(), 20.0 );
  ASSERT_EQUAL( col2.getBlue(), 30.0 );
  ASSERT_EQUAL( col2.getOpacity(), 0.3 );
  col1.setGreen( col2.getOpacity() );
  ASSERT_EQUAL( col1.getGreen(), 0.3 );

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

  SECTION( "Step" );
  {
    S3D::step stp1( S3D::the_origin, S3D::unit_threeVector_z );
    S3D::step stp2( makeThreeVector( 2.0, 1.0, 7.0 ), makeThreeVector( 3.5, 3.6, 12.4 ) );
    S3D::step stp3( makeThreeVector( 0.0, 0.0, 100.0 ), makeThreeVector( 0.0, 100.0, 0.0 ) );

    ASSERT_EQUAL( stp1.getStart(), S3D::the_origin );
    ASSERT_EQUAL( stp2.getStart(), makeThreeVector( 2.0, 1.0, 7.0 ) );
    ASSERT_EQUAL( stp3.getStart(), makeThreeVector( 0.0, 0.0, 100.0 ) );

    ASSERT_EQUAL( stp1.getCenter(), stp1.getPosition() );
    ASSERT_EQUAL( stp3.getCenter(), makeThreeVector( 0.0, 50.0, 100.0 ) );

    ASSERT_EQUAL( stp1.getDirection(), S3D::unit_threeVector_z );
    ASSERT_EQUAL( stp2.getDirection(), makeThreeVector( 3.5, 3.6, 12.4 ) );
    ASSERT_NOT_EQUAL( stp3.getDirection(), stp2.getDirection() );

    stp1.setDirection( S3D::unit_threeVector_y );
    stp2 = stp3;
    stp1.setCenter( S3D::the_origin );

    ASSERT_EQUAL( stp1.getDirection(), S3D::unit_threeVector_y );
    ASSERT_EQUAL( stp2.getStart(), stp3.getStart() );
    ASSERT_EQUAL( stp2.getPosition(), stp3.getPosition() );
    ASSERT_EQUAL( stp2.getDirection(), stp3.getDirection() );

    stp3.setStart( makeThreeVector( 0.3, 0.1, 30.0 ) );

    ASSERT_EQUAL( stp3.getStart(), makeThreeVector( 0.3, 0.1, 30.0 ) );
    ASSERT_EQUAL( stp1.getStart(), makeThreeVector( 0.0, -0.5, 0.0 ) );
  }

////////////////////////////////////////////////////////////////////////////////////////////////////

  SECTION( "Point" );

  S3D::point* p1 = S3D::addObject( new S3D::point( 0.0, 0.0, 0.0 ) );
  ASSERT_EQUAL( p1->getCenter()[0], 0.0 );
  ASSERT_EQUAL( p1->getCenter()[1], 0.0 );
  ASSERT_EQUAL( p1->getCenter()[2], 0.0 );

  S3D::point* p2 = S3D::addObject( new S3D::point( makeThreeVector( 1.0, 2.0, 3.0 ) ) );
  ASSERT_EQUAL( p2->getCenter()[0], 1.0 );
  ASSERT_EQUAL( p2->getCenter()[1], 2.0 );
  ASSERT_EQUAL( p2->getCenter()[2], 3.0 );

  S3D::point* p3 = S3D::addObject( new S3D::point( 15.7 ) );
  ASSERT_EQUAL( p3->getCenter()[0], 15.7 );
  ASSERT_EQUAL( p3->getCenter()[1], 15.7 );
  ASSERT_EQUAL( p3->getCenter()[2], 15.7 );

////////////////////////////////////////////////////////////////////////////////////////////////////

  SECTION( "Line" );

  S3D::line* l1 = S3D::addObject( new S3D::line( makeThreeVector( 1.0, 2.0, 3.0 ), makeThreeVector( 1.0, 1.0, 10.0 ) ) );
  S3D::line* l2 = S3D::addObject( new S3D::line( makeThreeVector( 1.0, 2.0, 4.0 ), makeThreeVector( 1.0, 1.0, 10.0 ) ) );
  ASSERT_EQUAL( l1->getStart(), makeThreeVector( 1.0, 2.0, 3.0 ) );
  ASSERT_EQUAL( l1->getDirection(), makeThreeVector( 1.0, 1.0, 10.0 ).norm() );
  ASSERT_EQUAL( l2->getStart(), makeThreeVector( 1.0, 2.0, 4.0 ) );
  ASSERT_EQUAL( l2->getDirection(), makeThreeVector( 1.0, 1.0, 10.0 ).norm() );
  ASSERT_APPROX_EQUAL( l1->getDirection().mod(), 1.0 );
  ASSERT_APPROX_EQUAL( l2->getDirection().mod(), 1.0 );
  ASSERT_EQUAL( l1->getDirection(), l2->getDirection() );
  ASSERT_APPROX_EQUAL( l1->distance( l2 ), 0.1400280084 );

////////////////////////////////////////////////////////////////////////////////////////////////////

  SECTION( "Surface" );

  threeVector test = makeThreeVector( 1.0, -1.0, std::sqrt(2.0) ).norm();
  S3D::test_surface* s1 = S3D::addObject( new S3D::test_surface( threeVector( 0.0 ), S3D::rotation( makeThreeVector( 1.0, 1.0, 0.0 ), 0.25*S3D::PI ) ) );

  ASSERT_EQUAL( s1->getCenter(), threeVector( 0.0 ) );
  ASSERT_APPROX_EQUAL( s1->getDirection()[0], test[0] );
  ASSERT_APPROX_EQUAL( s1->getDirection()[1], test[1] );
  ASSERT_APPROX_EQUAL( s1->getDirection()[2], test[2] );
  ASSERT_EQUAL( s1->inFront( makeThreeVector( 1.0, 2.0, 3.0 ) ), false );
  ASSERT_EQUAL( s1->crosses( l2 ), false );
  
////////////////////////////////////////////////////////////////////////////////////////////////////

  SECTION( "Simple Shapes" );

  S3D::test_simple_shape* sh1 = S3D::addObject( new S3D::test_simple_shape( 0, 1.0 ) );
  ASSERT_EQUAL( sh1->crosses( l1 ), false );
  ASSERT_EQUAL( sh1->contains( threeVector( 0.0 ) ), false );
  ASSERT_EQUAL( sh1->Contains( makeThreeVector( 1.0, 2.0, 3.0 ) ), false );

////////////////////////////////////////////////////////////////////////////////////////////////////

  if ( ! testass::control::summarize() )
  {
    testass::control::printReport( std::cout );
  }

  S3D::manager::killInstance();

  testass::control::kill();
  std::cout << std::endl;
  return 0;
}

