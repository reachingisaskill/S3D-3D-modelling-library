
#define S3D_TEST_FUNCTIONALITY

#define TESTASS_APPROX_LIMIT 1.0e-8

#include <iostream>

#include "testass.h"

#include "S3D.h"


int main( int, char** )
{
  testass::control::init( "S3D", "Simple Shape Intersection Tests" );
  testass::control::get()->setVerbosity( testass::control::verb_short );

  S3D::manager::createInstance();
  S3D::manager::getInstance()->setWorld( new S3D::box( 200.0, 200.0, 200.0, S3D::the_origin ) );


  // Throw some shapes

  S3D::sphere* sph1 = S3D::addObject( new S3D::sphere( 10.0, makeThreeVector( 0.0, 0.0, 0.0 ) ) );
  S3D::sphere* sph2 = S3D::addObject( new S3D::sphere( 10.0, makeThreeVector( 0.0, 0.0, 10.0 ) ) );
  S3D::sphere* sph3 = S3D::addObject( new S3D::sphere( 5.0, makeThreeVector( 5.0, 0.0, 0.0 ) ) );
  S3D::sphere* sph4 = S3D::addObject( new S3D::sphere( 5.0, makeThreeVector( 100.0, 0.0, 0.0 ) ) );
  S3D::sphere* sph5 = S3D::addObject( new S3D::sphere( 10.0, makeThreeVector( 0.0, 10.0, 0.0 ) ) );
  S3D::cylinder* cyl1 = S3D::addObject( new S3D::cylinder( 1.0, 5.0, makeThreeVector( 0.0, 0.0, 0.0 ) ) );
  S3D::cylinder* cyl2 = S3D::addObject( new S3D::cylinder( 10.0, 10.0, makeThreeVector( 0.0, 0.0, 5.0 ) ) );
  S3D::cylinder* cyl3 = S3D::addObject( new S3D::cylinder( 5.0, 10.0, makeThreeVector( -5.0, 0.0, 0.0 ), S3D::rotation( makeThreeVector( 0.0, 1.0, 0.0 ), S3D::PI*0.5 ) ) );
  S3D::cylinder* cyl4 = S3D::addObject( new S3D::cylinder( 5.0, 20.0, makeThreeVector( 85.0, 0.0, 0.0 ), S3D::rotation( makeThreeVector( 0.0, 1.0, 0.0 ), S3D::PI*0.5 ) ) );
  S3D::box* box1 = S3D::addObject( new S3D::box( 5.0, 5.0, 5.0, makeThreeVector( 0.0, 0.0, 0.0 ) ) );
  S3D::box* box2 = S3D::addObject( new S3D::box( 20.0, 20.0, 20.0, makeThreeVector( 0.0, 0.0, -15.0 ) ) );
  S3D::box* box3 = S3D::addObject( new S3D::box( 5.0, 5.0, 5.0, makeThreeVector( 0.0, 0.0, -2.5 ) ) );
  S3D::box* box4 = S3D::addObject( new S3D::box( 10.0, 5.0, 5.0, makeThreeVector( 105.0, 0.0, 0.0 ), S3D::rotation( makeThreeVector(1.0, 0.0, 0.0 ),  S3D::PI ) ) );



////////////////////////////////////////////////////////////////////////////////////////////////////

  SECTION( "Addition" );
  {
    S3D::addition* add1 = S3D::addObject( new S3D::addition( sph1, cyl1 ) );

    ASSERT_TRUE( add1->contains( &S3D::the_origin ) );

    S3D::addition* add2 = S3D::addObject( new S3D::addition( cyl3, sph3 ) );
    threeVector vec1 = makeThreeVector( 5.0, 0.0, 0.0 );
    threeVector vec2 = makeThreeVector( 9.99999, 0.0, 0.0 );
    threeVector vec3 = makeThreeVector( 5.0, 4.99999, 0.0 );
    threeVector vec4 = makeThreeVector( 5.0, 0.0, -4.999999 );
    threeVector vec5 = makeThreeVector( 0.000001, 0.0, 0.0 );
    threeVector vec6 = makeThreeVector( 10.000001, 0.0, 0.0 );
    threeVector vec7 = makeThreeVector( 5.0, 5.0000001, 0.0 );
    threeVector vec8 = makeThreeVector( 4.0, 2.0, 2.0 );

    ASSERT_TRUE( add2->contains( &vec1 ) );
    ASSERT_TRUE( add2->contains( &vec2 ) );
    ASSERT_TRUE( add2->contains( &vec3 ) );
    ASSERT_TRUE( add2->contains( &vec4 ) );
    ASSERT_TRUE( add2->contains( &vec5 ) );
    ASSERT_FALSE( add2->contains( &vec6 ) );
    ASSERT_FALSE( add2->contains( &vec7 ) );
    ASSERT_TRUE( add2->contains( &vec8 ) );
  }

////////////////////////////////////////////////////////////////////////////////////////////////////

  SECTION( "Subtraction" );
  {
    S3D::subtraction* sub1 = S3D::addObject( new S3D::subtraction( sph1, cyl1 ) );

    ASSERT_FALSE( sub1->contains( &S3D::the_origin ) );

    threeVector vec1 = makeThreeVector( 0.0, 5.0, 0.0 );
    threeVector vec2 = makeThreeVector( 0.0, 9.99999999, 0.0 );
    threeVector vec3 = makeThreeVector( 0.0, 5.0, 7.1 );
    threeVector vec4 = makeThreeVector( 0.0, 0.0, 5.0 );
    threeVector vec5 = makeThreeVector( 0.0, -0.000001, 0.0 );
    threeVector vec6 = makeThreeVector( 0.0, 10.00001, 0.0 );
    threeVector vec7 = makeThreeVector( 0.0, 20.000001, 0.0 );
    threeVector vec8 = makeThreeVector( 0.0, 10.0, 9.9999999 );

    S3D::subtraction* sub2 = S3D::addObject( new S3D::subtraction( sph1, sph5 ) );

    ASSERT_FALSE( sub2->contains( &vec1 ) );
    ASSERT_FALSE( sub2->contains( &vec2 ) );
    ASSERT_FALSE( sub2->contains( &vec3 ) );
    ASSERT_TRUE( sub2->contains( &vec4 ) );
    ASSERT_TRUE( sub2->contains( &vec5 ) );
    ASSERT_FALSE( sub2->contains( &vec6 ) );
    ASSERT_FALSE( sub2->contains( &vec7 ) );
    ASSERT_FALSE( sub2->contains( &vec8 ) );
  }

////////////////////////////////////////////////////////////////////////////////////////////////////

  SECTION( "Exclusion" );
  {
    S3D::exclusion* exc1 = S3D::addObject( new S3D::exclusion( sph1, cyl1 ) );

    ASSERT_FALSE( exc1->contains( &S3D::the_origin ) );

    S3D::exclusion* exc2 = S3D::addObject( new S3D::exclusion( sph1, sph5 ) );

    threeVector vec1 = makeThreeVector( 0.0, 5.0, 0.0 );
    threeVector vec2 = makeThreeVector( 0.0, 9.99999999, 0.0 );
    threeVector vec3 = makeThreeVector( 0.0, 5.0, 7.1 );
    threeVector vec4 = makeThreeVector( 0.0, 0.0, 5.0 );
    threeVector vec5 = makeThreeVector( 0.0, -0.000001, 0.0 );
    threeVector vec6 = makeThreeVector( 0.0, 10.00001, 0.0 );
    threeVector vec7 = makeThreeVector( 0.0, 20.000001, 0.0 );
    threeVector vec8 = makeThreeVector( 0.0, 10.0, 9.9999999 );

    ASSERT_FALSE( exc2->contains( &vec1 ) );
    ASSERT_FALSE( exc2->contains( &vec2 ) );
    ASSERT_FALSE( exc2->contains( &vec3 ) );
    ASSERT_TRUE( exc2->contains( &vec4 ) );
    ASSERT_TRUE( exc2->contains( &vec5 ) );
    ASSERT_TRUE( exc2->contains( &vec6 ) );
    ASSERT_FALSE( exc2->contains( &vec7 ) );
    ASSERT_TRUE( exc2->contains( &vec8 ) );
  }

////////////////////////////////////////////////////////////////////////////////////////////////////
  
  SECTION( "Intersection" );
  {
    S3D::intersection* int1 = S3D::addObject( new S3D::intersection( sph1, cyl1 ) );

    ASSERT_TRUE( int1->contains( &S3D::the_origin ) );

    threeVector vec1 = makeThreeVector( 0.0, 5.0, 0.0 );
    threeVector vec2 = makeThreeVector( 0.0, 9.99999999, 0.0 );
    threeVector vec3 = makeThreeVector( 0.0, 5.0, 10.0 );
    threeVector vec4 = makeThreeVector( 0.0, 0.0, 5.0 );
    threeVector vec5 = makeThreeVector( 0.0, -0.000001, 0.0 );
    threeVector vec6 = makeThreeVector( 0.0, 10.00001, 0.0 );
    threeVector vec7 = makeThreeVector( 0.0, 20.000001, 0.0 );
    threeVector vec8 = makeThreeVector( 0.0, 10.0, 9.9999999 );

    S3D::intersection* int2 = S3D::addObject( new S3D::intersection( sph1, sph5 ) );

    ASSERT_TRUE( int2->contains( &vec1 ) );
    ASSERT_TRUE( int2->contains( &vec2 ) );
    ASSERT_FALSE( int2->contains( &vec3 ) );
    ASSERT_FALSE( int2->contains( &vec4 ) );
    ASSERT_FALSE( int2->contains( &vec5 ) );
    ASSERT_FALSE( int2->contains( &vec6 ) );
    ASSERT_FALSE( int2->contains( &vec7 ) );
    ASSERT_FALSE( int2->contains( &vec8 ) );
  }

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


