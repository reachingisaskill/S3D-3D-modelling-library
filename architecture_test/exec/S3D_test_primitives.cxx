
#define TESTASS_APPROX_LIMIT 1.0e-8

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

using namespace S3D;


int main( int, char** )
{
  testass::control::init( "S3D", "Primitive Shape Tests" );
  testass::control::get()->setVerbosity( testass::control::verb_short );

  logtastic::setLogFileDirectory( "./test_data/" );
  logtastic::addLogFile( "./primitives_test.log" );
  logtastic::init( "Testing S3D Primitives", S3D_VERSION_NUMBER );

  S3D::manager::createInstance();

  S3D::manager* man = S3D::manager::getInstance();


  INFO_LOG( "Making basic material" );
  S3D::material_base* mat = (S3D::material_base*) new S3D::material_simple( S3D::colour( 1.0, 0.0, 0.0 ) );
  mat->setRefractiveIndex( 1.5 );
  man->addMaterial( "simple", mat );
  S3D::material_base* world_mat = (S3D::material_base*) new S3D::material_simple( S3D::colour( 0.0, 0.3, 0.8 ) );
  man->addMaterial( "room", world_mat );


  INFO_LOG( "Making world sphere" );
  S3D::object_base* world = (S3D::object_base*) new S3D::sphere( world_mat, 100.0 );
  man->setWorld( world, 1 );



////////////////////////////////////////////////////////////////////////////////////////////////////


  SECTION( "Sphere" );
  {
    point p1 = makeThreeVector( 0.0, 0.0, 0.0 );
    point p2 = makeThreeVector( 1.0, 1.0, 2.0 );
    point p3 = makeThreeVector( 1.0, 1.0, -8.0 );
    point p4 = makeThreeVector( 1.0, 1.0, -8.000001 );
    point p5 = makeThreeVector( 1.0, 11.0001, 2.0 );
    point p6 = makeThreeVector( 1000.0, 0.0, 0.0 );
    point p7 = makeThreeVector( 0.0, 0.0, -100.0 );
    point p8 = makeThreeVector( 0.0, 0.0, -80.0 );
    point p9 = makeThreeVector( 0.0, 0.0, -120.00000000001 );
    point p10 = makeThreeVector( 4.765, -6.34, -90.35 );

    line l1( p4, makeThreeVector( 0.0, 0.0, 10.0 ) );
    line l2( point( 1.0, 1.0, -7.0 ), makeThreeVector( 0.0, 0.0, 20.0 ) );
    line l3( p2, makeThreeVector( 20.0, 0.0, 0.0 ) );

    object_base* sph1 = (object_base*) new sphere( mat, 10.0 );
    sph1->setPosition( point( 1.0, 1.0, 2.0 ) );
    man->addObject( sph1 );

    object_base* sph2 = (object_base*) new sphere( mat, 20.0 );
    sph2->setPosition( point( 0.0, 0.0, -100.0 ) );
    man->addObject( sph2 );


    ASSERT_TRUE( sph1->contains( p1 ) );
    ASSERT_TRUE( sph1->contains( p2 ) );
    ASSERT_TRUE( sph1->contains( p3 ) );
    ASSERT_FALSE( sph1->contains( p4 ) );
    ASSERT_FALSE( sph1->contains( p5 ) );
    ASSERT_FALSE( sph1->contains( p6 ) );
    ASSERT_FALSE( sph1->contains( p7 ) );
    ASSERT_FALSE( sph1->contains( p8 ) );
    ASSERT_FALSE( sph1->contains( p9 ) );
    ASSERT_FALSE( sph1->contains( p10 ) );

    ASSERT_FALSE( sph2->contains( p1 ) );
    ASSERT_FALSE( sph2->contains( p2 ) );
    ASSERT_FALSE( sph2->contains( p3 ) );
    ASSERT_FALSE( sph2->contains( p4 ) );
    ASSERT_FALSE( sph2->contains( p5 ) );
    ASSERT_FALSE( sph2->contains( p6 ) );
    ASSERT_TRUE( sph2->contains( p7 ) );
    ASSERT_TRUE( sph2->contains( p8 ) );
    ASSERT_FALSE( sph2->contains( p9 ) );
    ASSERT_TRUE( sph2->contains( p10 ) );

    ASSERT_TRUE( sph1->crosses( l1 ) );
    ASSERT_TRUE( sph1->crosses( l2 ) );
    interaction inter = sph1->intersect( l1 );

    ASSERT_APPROX_EQUAL( inter.getPoint()[0], 1.0 );
    ASSERT_APPROX_EQUAL( inter.getPoint()[1], 1.0 );
    ASSERT_APPROX_EQUAL( inter.getPoint()[2], -8.0 );

    ASSERT_APPROX_EQUAL( inter.getSurfaceNormal()[0], 0.0 );
    ASSERT_APPROX_EQUAL( inter.getSurfaceNormal()[1], 0.0 );
    ASSERT_APPROX_EQUAL( inter.getSurfaceNormal()[2], -1.0 );

    ASSERT_APPROX_EQUAL( inter.getTransmission()[0], 0.0 );
    ASSERT_APPROX_EQUAL( inter.getTransmission()[1], 0.0 );
    ASSERT_APPROX_EQUAL( inter.getTransmission()[2], 1.0 );

    inter = sph1->intersect( l2 );
    ASSERT_APPROX_EQUAL( inter.getPoint()[0], 1.0 );
    ASSERT_APPROX_EQUAL( inter.getPoint()[1], 1.0 );
    ASSERT_APPROX_EQUAL( inter.getPoint()[2], 12.0 );

    ASSERT_APPROX_EQUAL( inter.getSurfaceNormal()[0], 0.0 );
    ASSERT_APPROX_EQUAL( inter.getSurfaceNormal()[1], 0.0 );
    ASSERT_APPROX_EQUAL( inter.getSurfaceNormal()[2], -1.0 );

    ASSERT_APPROX_EQUAL( inter.getTransmission()[0], 0.0 );
    ASSERT_APPROX_EQUAL( inter.getTransmission()[1], 0.0 );
    ASSERT_APPROX_EQUAL( inter.getTransmission()[2], 1.0 );

    inter = sph1->intersect( l3 );
    ASSERT_APPROX_EQUAL( inter.getPoint()[0], 11.0 );
    ASSERT_APPROX_EQUAL( inter.getPoint()[1], 1.0 );
    ASSERT_APPROX_EQUAL( inter.getPoint()[2], 2.0 );

    ASSERT_APPROX_EQUAL( inter.getSurfaceNormal()[0], -1.0 );
    ASSERT_APPROX_EQUAL( inter.getSurfaceNormal()[1], 0.0 );
    ASSERT_APPROX_EQUAL( inter.getSurfaceNormal()[2], 0.0 );
  }


////////////////////////////////////////////////////////////////////////////////////////////////////

  SECTION( "Box" );
  {
    point p1 = makeThreeVector( 0.0, 0.0, 0.0 );
    point p2 = makeThreeVector( 0.5, 0.0, 0.0 );
    point p3 = makeThreeVector( 0.0, 1.1, 0.0 );
    point p4 = makeThreeVector( 0.0, 0.0, 1.6 );
    point p5 = makeThreeVector( 0.5, 1.0, 1.5 );
    point p6 = makeThreeVector( 100.0, 0.0, 0.0 );
    point p7 = makeThreeVector( 125.0, 0.0, 0.0 );
    point p8 = makeThreeVector( 125.0000001, 0.0, 0.0 );
    point p9 = makeThreeVector( 100.0, 14.1421355, -14.1421355 ); // Just inside
    point p10 = makeThreeVector( 100.0, 10.6066017, 10.6066017 );
    point p11 = makeThreeVector( 100.0, 14.1421357, -14.1421357 ); // Just outside
    point p12 = makeThreeVector( 100.0, 10.6066019, 10.6066019 );

    rotation r1( -S3D::unit_threeVector_x, 0.25*S3D::PI );

    box* b1 = new S3D::box( mat, 1.0, 2.0, 3.0 );
    b1->setPosition( p1 );
    man->addObject( (object_base*) b1 );
  
    box* b2 = new S3D::box( mat, 50.0, 40.0, 30.0 );
    b2->setPosition( p6 );
    b2->setRotation( r1 );
    man->addObject( (object_base*) b2 );

    threeVector v1 = S3D::defaultDirection;
    threeVector v2 = r1 * S3D::defaultDirection;

    line l1( p1, v1 );
    line l2( p6, makeThreeVector( -1.0, 0.0, 0.0 ) );
    line l3( point( 0.49999, 0.0, 0.0) , v1 );
    line l4( point( 0.50001, 0.0, 0.0) , v1 );


    // Surface Centers
    ASSERT_APPROX_EQUAL( b1->getSurfaces()[0].getPosition()[0], 0.5 );
    ASSERT_APPROX_EQUAL( b1->getSurfaces()[0].getPosition()[1], 0.0 );
    ASSERT_APPROX_EQUAL( b1->getSurfaces()[0].getPosition()[2], 0.0 );

    ASSERT_APPROX_EQUAL( b1->getSurfaces()[1].getPosition()[0], -0.5 );
    ASSERT_APPROX_EQUAL( b1->getSurfaces()[1].getPosition()[1], 0.0 );
    ASSERT_APPROX_EQUAL( b1->getSurfaces()[1].getPosition()[2], 0.0 );

    ASSERT_APPROX_EQUAL( b1->getSurfaces()[2].getPosition()[0], 0.0 );
    ASSERT_APPROX_EQUAL( b1->getSurfaces()[2].getPosition()[1], 1.0 );
    ASSERT_APPROX_EQUAL( b1->getSurfaces()[2].getPosition()[2], 0.0 );

    ASSERT_APPROX_EQUAL( b1->getSurfaces()[3].getPosition()[0], 0.0 );
    ASSERT_APPROX_EQUAL( b1->getSurfaces()[3].getPosition()[1], -1.0 );
    ASSERT_APPROX_EQUAL( b1->getSurfaces()[3].getPosition()[2], 0.0 );

    ASSERT_APPROX_EQUAL( b1->getSurfaces()[4].getPosition()[0], 0.0 );
    ASSERT_APPROX_EQUAL( b1->getSurfaces()[4].getPosition()[1], 0.0 );
    ASSERT_APPROX_EQUAL( b1->getSurfaces()[4].getPosition()[2], 1.5 );

    ASSERT_APPROX_EQUAL( b1->getSurfaces()[5].getPosition()[0], 0.0 );
    ASSERT_APPROX_EQUAL( b1->getSurfaces()[5].getPosition()[1], 0.0 );
    ASSERT_APPROX_EQUAL( b1->getSurfaces()[5].getPosition()[2], -1.5 );


    // Directions
    ASSERT_APPROX_EQUAL( ( b2->getSurfaces()[0].getPosition() - point( 125.0, 0.0, 0.0 ) ).mod(), 0.0 );
    ASSERT_APPROX_EQUAL( ( b2->getSurfaces()[1].getPosition() - point( 75.0, 0.0, 0.0 ) ).mod(), 0.0 );
    ASSERT_APPROX_EQUAL( ( b2->getSurfaces()[2].getPosition() - point( 100.0, 14.14213562, -14.14213562 ) ).mod(), 0.0 );
    ASSERT_APPROX_EQUAL( ( b2->getSurfaces()[3].getPosition() - point( 100.0, -14.14213562, 14.14213562 ) ).mod(), 0.0 );
    ASSERT_APPROX_EQUAL( ( b2->getSurfaces()[4].getPosition() - point( 100.0, 10.60660172, 10.60660172 ) ).mod(), 0.0 );
    ASSERT_APPROX_EQUAL( ( b2->getSurfaces()[5].getPosition() - point( 100.0, -10.60660172, -10.60660172 ) ).mod(), 0.0 );

    ASSERT_APPROX_EQUAL( ( b2->getSurfaces()[0].getNormal() - unit_threeVector_x ).mod(), 0.0 );
    ASSERT_APPROX_EQUAL( ( b2->getSurfaces()[1].getNormal() + unit_threeVector_x ).mod(), 0.0 );
    ASSERT_APPROX_EQUAL( ( b2->getSurfaces()[2].getNormal() - makeThreeVector( 0.0, 1.0, -1.0 ).norm() ).mod(), 0.0 );
    ASSERT_APPROX_EQUAL( ( b2->getSurfaces()[3].getNormal() - makeThreeVector( 0.0, -1.0, 1.0 ).norm() ).mod(), 0.0 );
    ASSERT_APPROX_EQUAL( ( b2->getSurfaces()[4].getNormal() - makeThreeVector( 0.0, 1.0, 1.0 ).norm() ).mod(), 0.0 );
    ASSERT_APPROX_EQUAL( ( b2->getSurfaces()[5].getNormal() - makeThreeVector( 0.0, -1.0, -1.0 ).norm() ).mod(), 0.0 );


    // Point Containment
    ASSERT_TRUE( b1->contains( p1 ) );
    ASSERT_TRUE( b1->contains( p2 ) );
    ASSERT_FALSE( b1->contains( p3 ) );
    ASSERT_FALSE( b1->contains( p4 ) );
    ASSERT_TRUE( b1->contains( p5 ) );
    ASSERT_FALSE( b1->contains( p6 ) );
    ASSERT_FALSE( b1->contains( p7 ) );
    ASSERT_FALSE( b1->contains( p8 ) );
    ASSERT_FALSE( b1->contains( p9 ) );
    ASSERT_FALSE( b1->contains( p10 ) );

    ASSERT_FALSE( b2->contains( p1 ) );
    ASSERT_FALSE( b2->contains( p2 ) );
    ASSERT_FALSE( b2->contains( p3 ) );
    ASSERT_FALSE( b2->contains( p4 ) );
    ASSERT_FALSE( b2->contains( p5 ) );
    ASSERT_TRUE( b2->contains( p6 ) );
    ASSERT_TRUE( b2->contains( p7 ) );
    ASSERT_FALSE( b2->contains( p8 ) );
    ASSERT_TRUE( b2->contains( p9 ) );
    ASSERT_TRUE( b2->contains( p10 ) );
    ASSERT_FALSE( b2->contains( p11 ) );
    ASSERT_FALSE( b2->contains( p12 ) );


    ASSERT_TRUE( b1->crosses( l1 ) );
    ASSERT_TRUE( b2->crosses( l2 ) );
    ASSERT_FALSE( b2->crosses( l1 ) );
    ASSERT_TRUE( b2->crosses( l2 ) );
    interaction inter = b1->intersect( l1 );

    ASSERT_APPROX_EQUAL( inter.getPoint()[0], 0.0 );
    ASSERT_APPROX_EQUAL( inter.getPoint()[1], 0.0 );
    ASSERT_APPROX_EQUAL( inter.getPoint()[2], 1.5 );

    ASSERT_APPROX_EQUAL( inter.getSurfaceNormal()[0], 0.0 );
    ASSERT_APPROX_EQUAL( inter.getSurfaceNormal()[1], 0.0 );
    ASSERT_APPROX_EQUAL( inter.getSurfaceNormal()[2], -1.0 );

    inter = b1->intersect( l2 );
    ASSERT_APPROX_EQUAL( inter.getPoint()[0], 0.5 );
    ASSERT_APPROX_EQUAL( inter.getPoint()[1], 0.0 );
    ASSERT_APPROX_EQUAL( inter.getPoint()[2], 0.0 );

    ASSERT_APPROX_EQUAL( inter.getSurfaceNormal()[0], 1.0 );
    ASSERT_APPROX_EQUAL( inter.getSurfaceNormal()[1], 0.0 );
    ASSERT_APPROX_EQUAL( inter.getSurfaceNormal()[2], 0.0 );

    inter = b2->intersect( l2 );
    ASSERT_APPROX_EQUAL( inter.getPoint()[0], p6[0]-std::sqrt( 20.0*20.0 + 15.0*15.0 ) );
    ASSERT_APPROX_EQUAL( inter.getPoint()[1], 0.0 );
    ASSERT_APPROX_EQUAL( inter.getPoint()[2], 0.0 );
    
    ASSERT_TRUE( b1->crosses( l3 ) );
    inter = b1->intersect( l3 );
    ASSERT_APPROX_EQUAL( inter.getPoint()[0], 0.49999 );
    ASSERT_APPROX_EQUAL( inter.getPoint()[1], 0.0 );
    ASSERT_APPROX_EQUAL( inter.getPoint()[2], 1.5 );

    ASSERT_FALSE( b2->crosses( l4 ) );
  }


  SECTION( "Square Planar Surface" );
  {
    point p1( 0.0, 0.0, 1.0 );
    point p2( 0.0, 0.0, -1.0 );
    point p3( 5.000001, 0.0, 0.0 );
    point p4( 0.0, 0.0, 0.0 );
    point p5( 0.0, 0.0, 0.0 );

    rotation r1( unit_threeVector_y, degreesToRadians( 45.0 ) );

    line l1( p1, makeThreeVector( 0.0, 0.0, -1.0 ) );
    line l2( p2, makeThreeVector( 0.0, 0.0, 1.0 ) );
    line l3( p1, makeThreeVector( 0.0, 0.0, 1.0 ) );
    line l4( p3, makeThreeVector( 0.0, 0.0, -1.0 ) );

    square_plane* sqp = new square_plane( mat, 10.0, 10.0 );
    sqp->setPosition( point( 0.0, 5.0, 0.0 ) );
    man->addObject( (object_base*) sqp );

    ASSERT_FALSE( sqp->contains( p1 ) );
    ASSERT_TRUE( sqp->contains( p2 ) );

    ASSERT_TRUE( sqp->crosses( l1 ) );
    ASSERT_TRUE( sqp->crosses( l2 ) );
    ASSERT_FALSE( sqp->crosses( l3 ) );
    ASSERT_FALSE( sqp->crosses( l4 ) );

    interaction inter = sqp->intersect( l1 );
    ASSERT_APPROX_EQUAL( inter.getPoint()[0], 0.0 );
    ASSERT_APPROX_EQUAL( inter.getPoint()[1], 0.0 );
    ASSERT_APPROX_EQUAL( inter.getPoint()[2], 0.0 );

    ASSERT_APPROX_EQUAL( inter.getSurfaceNormal()[0], 0.0 );
    ASSERT_APPROX_EQUAL( inter.getSurfaceNormal()[1], 0.0 );
    ASSERT_APPROX_EQUAL( inter.getSurfaceNormal()[2], 1.0 );

    inter = sqp->intersect( l2 );
    ASSERT_APPROX_EQUAL( inter.getPoint()[0], 0.0 );
    ASSERT_APPROX_EQUAL( inter.getPoint()[1], 0.0 );
    ASSERT_APPROX_EQUAL( inter.getPoint()[2], 0.0 );

    ASSERT_APPROX_EQUAL( inter.getSurfaceNormal()[0], 0.0 );
    ASSERT_APPROX_EQUAL( inter.getSurfaceNormal()[1], 0.0 );
    ASSERT_APPROX_EQUAL( inter.getSurfaceNormal()[2], -1.0 );

    sqp->setRotation( r1 );

    inter = sqp->intersect( l2 );
    ASSERT_APPROX_EQUAL( inter.getPoint()[0], 0.0 );
    ASSERT_APPROX_EQUAL( inter.getPoint()[1], 0.0 );
    ASSERT_APPROX_EQUAL( inter.getPoint()[2], 0.0 );

    ASSERT_APPROX_EQUAL( inter.getSurfaceNormal()[0], -1.0/std::sqrt(2.0) );
    ASSERT_APPROX_EQUAL( inter.getSurfaceNormal()[1], 0.0 );
    ASSERT_APPROX_EQUAL( inter.getSurfaceNormal()[2], -1.0/std::sqrt(2.0) );



        
  }

//////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  SECTION( "Cylinder" );
//
//  threeVector p1 = makeThreeVector( 0.0, 0.0, 0.0 );
//  threeVector p2 = makeThreeVector( 0.0, 0.0, 5.0 );
//  threeVector p3 = makeThreeVector( 2.0, 0.0, 0.0 );
//  threeVector p4 = makeThreeVector( 0.0, -2.0, -5.0 );
//  threeVector p5 = makeThreeVector( 0.0, 0.0, 5.00000000001 );
//  threeVector p6 = makeThreeVector( -2.0000000000001, 0.0, 0.0 );
//  threeVector p7 = makeThreeVector( 100.0, 0.0, 0.0 );
//  threeVector p8 = makeThreeVector( 107.071067811865475, -7.071067811865475, 0.0 ); // Just Inside
//  threeVector p9 = makeThreeVector( 100.0, 14.1421356237, 0.0 );
//  threeVector p10 = makeThreeVector( 107.07106781188, -7.07106781188, 0.0 ); // Just Outside
//  threeVector p11 = makeThreeVector( 100.0, 14.1421356239, 0.0 );
//  // threeVector p12 = makeThreeVector( 114.1421356237, 14.1421356237, 0.0 );
//  threeVector p12 = makeThreeVector( 107.07106781185, -7.07106781185, 0.0 ); // Inside
//  threeVector p13 = makeThreeVector( 107.07106781188, -7.07106781188, 0.0 ); // Outside
//  threeVector p14 = makeThreeVector( 109.0, 9.0, 0.0 );
//
//  S3D::rotation r1( makeThreeVector( 0.0, 0.0, 1.0 ), 0.5*S3D::PI );
//  S3D::rotation r2( makeThreeVector( 1.0, 1.0, 0.0 ), 0.5*S3D::PI );
//
//  S3D::cylinder* c1 = S3D::addObject( new S3D::cylinder( 2.0, 10.0, p1 ) );
//  S3D::cylinder* c2 = S3D::addObject( new S3D::cylinder( 2.0, 20.0, p1, r1 ) );
//  S3D::cylinder* c3 = S3D::addObject( new S3D::cylinder( 10.0, 20.0, p7, r2 ) );
//
//  ASSERT_EQUAL( c1->getDirection(), c2->getDirection() );
//  ASSERT_EQUAL( c1->getCenter(), p1 );
//
//  // Surface Centers
//  ASSERT_APPROX_EQUAL( ( c1->surfaces()[0]->getCenter() - makeThreeVector( 0.0, 0.0, 5.0 ) ).mod(), 0.0 );
//  ASSERT_APPROX_EQUAL( ( c1->surfaces()[1]->getCenter() - makeThreeVector( 0.0, 0.0, 0.0 ) ).mod(), 0.0 );
//  ASSERT_APPROX_EQUAL( ( c1->surfaces()[2]->getCenter() - makeThreeVector( 0.0, 0.0, -5.0 ) ).mod(), 0.0 );
//
//  ASSERT_APPROX_EQUAL( ( c3->surfaces()[0]->getCenter() - makeThreeVector( 100.0 + 7.0710678118654755, -7.0710678118654755, 0.0 ) ).mod(), 0.0 );
//  ASSERT_APPROX_EQUAL( ( c3->surfaces()[1]->getCenter() - makeThreeVector( 100.0, 0.0, 0.0 ) ).mod(), 0.0 );
//  ASSERT_APPROX_EQUAL( ( c3->surfaces()[2]->getCenter() - makeThreeVector( 100.0 - 7.0710678118654755, 7.0710678118654755, 0.0 ) ).mod(), 0.0 );
//
//  // Surface Directions
//
//  ASSERT_APPROX_EQUAL( ( c1->surfaces()[0]->getDirection() - S3D::defaultDirection ).mod(), 0.0 );
//  ASSERT_APPROX_EQUAL( ( c1->surfaces()[1]->getDirection() + c1->surfaces()[2]->getDirection() ).mod(), 0.0 );
//  ASSERT_EQUAL( c1->surfaces()[0]->getDirection(), c1->surfaces()[1]->getDirection() );
//
//  ASSERT_APPROX_EQUAL( ( c3->surfaces()[0]->getDirection() - makeThreeVector( 1.0, -1.0, 0.0 ).norm() ).mod(), 0.0 );
//  ASSERT_APPROX_EQUAL( ( c3->surfaces()[1]->getDirection() + c3->surfaces()[2]->getDirection() ).mod(), 0.0 );
//  ASSERT_EQUAL( c3->surfaces()[0]->getDirection(), c3->surfaces()[1]->getDirection() );
//
//  // Point Containement
//  
//  ASSERT_TRUE( c1->contains( &p1 ) );
//  ASSERT_TRUE( c1->Contains( &p1 ) );
//  ASSERT_TRUE( c1->contains( &p2 ) );
//  ASSERT_FALSE( c1->Contains( &p2 ) );
//  ASSERT_TRUE( c1->contains( &p3 ) );
//  ASSERT_FALSE( c1->Contains( &p3 ) );
//  ASSERT_TRUE( c1->contains( &p4 ) );
//  ASSERT_FALSE( c1->contains( &p5 ) );
//  ASSERT_FALSE( c1->Contains( &p5 ) );
//  ASSERT_FALSE( c1->contains( &p6 ) );
//  ASSERT_FALSE( c1->Contains( &p6 ) );
//  ASSERT_FALSE( c1->contains( &p7 ) );
//  ASSERT_FALSE( c1->contains( &p8 ) );
//  ASSERT_FALSE( c1->contains( &p9 ) );
//  ASSERT_FALSE( c1->contains( &p10 ) );
//  ASSERT_FALSE( c1->contains( &p11 ) );
//  ASSERT_FALSE( c1->contains( &p12 ) );
//  ASSERT_FALSE( c1->contains( &p13 ) );
//
//  ASSERT_FALSE( c3->contains( &p1 ) );
//  ASSERT_FALSE( c3->contains( &p2 ) );
//  ASSERT_FALSE( c3->contains( &p3 ) );
//  ASSERT_FALSE( c3->contains( &p4 ) );
//  ASSERT_FALSE( c3->contains( &p5 ) );
//  ASSERT_FALSE( c3->contains( &p6 ) );
//  ASSERT_TRUE( c3->contains( &p7 ) );
//  ASSERT_TRUE( c3->Contains( &p7 ) );
//  ASSERT_TRUE( c3->contains( &p8 ) );
//  ASSERT_TRUE( c3->Contains( &p8 ) );
//  ASSERT_TRUE( c3->contains( &p9 ) );
//  ASSERT_TRUE( c3->Contains( &p9 ) );
//  ASSERT_FALSE( c3->contains( &p10 ) );
//  ASSERT_FALSE( c3->Contains( &p10 ) );
//  ASSERT_FALSE( c3->contains( &p11 ) );
//  ASSERT_FALSE( c3->Contains( &p11 ) );
//  ASSERT_TRUE( c3->contains( &p12 ) );
//  ASSERT_FALSE( c3->contains( &p13 ) );
//  ASSERT_FALSE( c3->contains( &p14 ) );
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////////

  if ( ! testass::control::summarize() )
  {
    testass::control::printReport( std::cout );
  }


  S3D::manager::killInstance();

  testass::control::kill();
  logtastic::stop();
  return 0;
}

