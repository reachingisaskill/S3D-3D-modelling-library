
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

  logtastic::init();
  logtastic::setLogFileDirectory( "./test_data/" );
  logtastic::addLogFile( "./primitives_test.log" );
  logtastic::start( "Testing S3D Primitives", S3D_VERSION_NUMBER );

  S3D::manager::createInstance();

  S3D::manager* man = S3D::manager::getInstance();


  INFO_LOG( "Making basic material" );
  S3D::material_base* mat = (S3D::material_base*) new S3D::material_simple( S3D::spectrum( 1.0, 0.0, 0.0 ) );
  mat->setRefractiveIndex( 1.5 );
  man->addMaterial( "simple", mat );
  S3D::material_base* world_mat = (S3D::material_base*) new S3D::material_simple( S3D::spectrum( 0.0, 0.3, 0.8 ) );
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
    

    object_base* sph3 = (object_base*) new sphere( mat, 1.0 );
    sph3->setPosition( point( 0.0, 0.0, 0.0 ) );
    man->addObject( sph3 );

    unsigned int counter_pos = 0;
    unsigned int counter_dir = 0;
    for ( unsigned int i = 0; i < 1000000; ++i ) // Sample the plane and make sure they point the right direction!
    {
      line test = sph3->sampleEmission();

      if ( std::fabs( test.getStart().getPosition().mod() - 1.0 ) > TESTASS_APPROX_LIMIT ) // Should all lie on unit sphere
        counter_pos += 1;

      threeVector normal = test.getStart().getPosition(); // Same as position vector
      if ( ( test.getDirection() * normal ) < 0.0 ) // if going backwards
        counter_dir += 1;
    }

    ASSERT_EQUAL( counter_pos, (unsigned int) 0 );
    ASSERT_EQUAL( counter_dir, (unsigned int) 0 );
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

////////////////////////////////////////////////////////////////////////////////////////////////////
  // Square Planar Surfaace

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

    square_plane* sqp2 = new square_plane( mat, 1.0, 1.0 );
    sqp2->setPosition( point( 0.0, 5.0, 0.0 ) );
    sqp2->setRotation( rotation( unit_threeVector_x, 2.0*PI/3.0 ) );
    man->addObject( (object_base*) sqp2 );

    unsigned int counter1 = 0;
    unsigned int counter2 = 0;
    threeVector normal_1 = r1.rotateVector( defaultDirection );
    threeVector normal_2 = rotation( unit_threeVector_x, 2.0*PI/3.0 ).rotateVector( defaultDirection );
    for ( unsigned int i = 0; i < 1000000; ++i ) // Sample the plane and make sure they point the right direction!
    {
      line test1 = sqp->sampleEmission();
      line test2 = sqp2->sampleEmission();

      if ( ( test1.getDirection() * normal_1 ) < 0.0 ) // if going backwards
        counter1 += 1;
      if ( ( test2.getDirection() * normal_2 ) < 0.0 ) // if going backwards
        counter2 += 1;
    }

    ASSERT_EQUAL( counter1, (unsigned int)0 );
    ASSERT_EQUAL( counter2, (unsigned int)0 );


        
  }

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

