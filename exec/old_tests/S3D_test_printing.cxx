
#include <iostream>

#include "S3D.h"

int main( int, char** )
{
  std::cout << "Testing printing functionality\n";
  std::cout << "Inventing some shapes...\n" << std::endl;

  S3D::printer printer( "test.wrl" );

  printer.addData( "Name", "Chris");
  printer.addData( "Content", "Fuckall!" );

  /*
  S3D::printShape shape1( "Cylinder" );

  shape1.addParam( "height", "2.0" );
  shape1.addParam( "radius", "1.0" );
  shape1.addParam( "side", "TRUE" );
  shape1.addParam( "top", "TRUE" );
  shape1.addParam( "bottom", "TRUE" );
  shape1.setPosition( makeThreeVector( -1.0, -1.0, 1.5 ) );

  S3D::printShape shape2( "Box" );
  shape2.addParam( "size", "2.0 1.0 3.0" );

  shape2.setPosition( makeThreeVector( 10.0, 0.0, 0.0 ) );
  shape2.setColour( 1.0, 0.0, 0.0 );

  printer.addShape( shape1 );
  printer.addShape( shape2 );
  */

  S3D::box b1( 10.0, 5.0, 7.0 );
  S3D::box b2( 2.0, 2.0, 2.0, makeThreeVector( 5.0, 15.0, 0.0 ) );

  S3D::cylinder c1( makeThreeVector( -5.0, -23.0, 8.0 ), S3D::unit_threeVector_z, 2.0, 7.0 );

  S3D::ray r1( makeThreeVector( -10.0, -10.0, -10.0 ), makeThreeVector( 10.0, 10.0, 10.0 ) );

  stdexts::sharedPtr< S3D::volume_base > SP1 = new S3D::box( 50.0, 5.0, 5.0, makeThreeVector(-20.0, -2.0, -1.0 ) );
  stdexts::sharedPtr< S3D::volume_base > SP2 = new S3D::box( 50.0, 5.0, 5.0, makeThreeVector(-20.0, +2.0, +1.0 ) );
  S3D::addition BS1 =  SP1 + SP2; 

  stdexts::sharedPtr< S3D::volume_base > SP3 = new S3D::cylinder( makeThreeVector(+20.0, -2.0, -2.0 ), S3D::unit_threeVector_z, 5.0, 5.0 );
  stdexts::sharedPtr< S3D::volume_base > SP4 = new S3D::cylinder( makeThreeVector(+20.0, -2.0, +2.0 ), S3D::unit_threeVector_z, 5.0, 5.0 );
  S3D::subtraction BS2 = SP3 - SP4; 

  c1.setOpacity( 0.1 );
  r1.setOpacity( 0.1 );
  BS2.setColour( 1.0, 0.6, 0.0 );

  b1.print( printer );
  b2.print( printer );
  c1.print( printer );
  r1.print( printer );

  BS1.print( printer );
  BS2.print( printer );

  printer.write();

  return 0;
}

