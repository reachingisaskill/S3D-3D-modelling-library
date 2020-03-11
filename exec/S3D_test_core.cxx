
#include "S3D.h"


#include "testass.h"
#include <iostream>



// Needed for testass
std::ostream& operator<< ( std::ostream&, S3D::colour );


void addSomeShapes();

int main( int, char** )
{
  testass::control::init( "S3D", "Core Functionalities - Memory management, etc" );
  testass::control::get()->setVerbosity( testass::control::verb_short );


  SECTION( "Manager - Basic Functions" );
  {
    S3D::colour testColour1( 1.0, 0.54, 234, 0.666 );
    S3D::colour testColour2( 1.0, 0.54, 234, 0.666 );

    S3D::manager::createInstance();

    S3D::manager* man = S3D::manager::getInstance();

    man->setDefaultColour( 1, testColour1 );
    man->setDefaultColour( 2, testColour2 );
    man->setDefaultColour( -645, testColour1 );
    ASSERT_EQUAL( testColour1, man->getDefaultColour( 1 ) );
    ASSERT_EQUAL( testColour2, man->getDefaultColour( 2 ) );
    ASSERT_EQUAL( testColour1, man->getDefaultColour( -645 ) );
    ASSERT_EQUAL( S3D_DEFAULT_COLOUR, man->getDefaultColour( 10353 ) );

    ASSERT_EQUAL( 0U, man->countObjects() );

    man->setLayerVisible( 52 );
    man->setLayerVisible( 3435 );
    man->setLayerVisible( -453 );
    man->setLayerInvisible( 52 );

    ASSERT_TRUE( man->getLayerVisible( 3435 ) );
    ASSERT_TRUE( man->getLayerVisible( -453 ) );
    ASSERT_FALSE( man->getLayerVisible( 52 ) );
    ASSERT_FALSE( man->getLayerVisible( -234 ) );

    S3D::box* world = new S3D::box( 10, 10, 10, S3D::the_origin );
    man->setWorld( world ); //S3D now looks after this memory!

    ASSERT_EQUAL( world, man->getWorld() );
    ASSERT_TRUE( man->worldContains( S3D::the_origin ) );

    S3D::manager::killInstance();
  }

  SECTION( "Manager - Memory Management" );
  {
    S3D::manager::createInstance();
    S3D::manager* man = S3D::manager::getInstance();

    S3D::box* world = new S3D::box( 100.0, 100.0, 100.0, S3D::the_origin );
    S3D::sphere* shape = new S3D::sphere( 1.0, S3D::the_origin );

    ASSERT_EQUAL( (S3D::object_base*)0, man->getWorld() );
    ASSERT_EQUAL( 0U, man->countObjects() );

    man->setWorld( world );
    ASSERT_EQUAL( 1U, man->countObjects() );
    ASSERT_EQUAL( world, man->getWorld() );
    ASSERT_TRUE( man->worldContains( S3D::the_origin ) );
    ASSERT_FALSE( man->worldContains( makeThreeVector( 1000.0, 0.0, 0.0 ) ) );

    man->addObject( world, 34);
    ASSERT_EQUAL( 1U, man->countObjects() );
    man->addObject( shape, 54);
    ASSERT_EQUAL( 2U, man->countObjects() );
    man->addObject( shape, 7 );
    ASSERT_EQUAL( 2U, man->countObjects() );
    man->removeObject( shape ); // Deletes it!
    ASSERT_EQUAL( 1U, man->countObjects() );

    addSomeShapes(); // Adds 6

    ASSERT_EQUAL( 7U, man->countObjects() );

    S3D::manager::killInstance();
  }


  SECTION( "Manager - Colouring and Printing - TBC" );
  {
  }


  if ( ! testass::control::summarize() )
  {
    testass::control::printReport( std::cout );
  }

  testass::control::kill();
  std::cout << std::endl;
  return 0;
}

void addSomeShapes()
{
  S3D::addObject( new S3D::box( 1.0, 1.0, 1.0, S3D::the_origin ) );
  S3D::addObject( new S3D::box( 1.0, 1.0, 1.0, S3D::the_origin ) );
  S3D::addObject( new S3D::box( 1.0, 1.0, 1.0, S3D::the_origin ) );
  S3D::addObject( new S3D::box( 1.0, 1.0, 1.0, S3D::the_origin ) );
  S3D::addObject( new S3D::box( 1.0, 1.0, 1.0, S3D::the_origin ) );
  S3D::addObject( new S3D::box( 1.0, 1.0, 1.0, S3D::the_origin ) );
}


std::ostream& operator<<( std::ostream& os, S3D::colour col )
{
  os << col.getRed() << ", " << col.getGreen() << ", " << col.getBlue() << ", " << col.getOpacity();
  return os;
}


