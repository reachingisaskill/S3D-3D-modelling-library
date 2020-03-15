
#include <iostream>
#include <string>

#include "S3D.h"

#include "testass.h"


int main( int, char** )
{

  testass::control::init( "S3D", "Line Scan Rendering" );
//  testass::control::get()->setVerbosity( testass::controll::verb_short );

  S3D::manager::createInstance();
  S3D::manager::getInstance()->setWorld( new S3D::box( 20, 20, 20, S3D::the_origin ) );

  S3D::sphere* sph1 = new S3D::sphere( 3.0, makeThreeVector( 2.0, 0.0, 2.0 ) );
  S3D::box* box1 = new S3D::box( 1.0, 1.0, 1.0, makeThreeVector( -5.0, 0.0, 0.0 ) );

  S3D::addObject( sph1 );
  S3D::addObject( box1 );

  S3D::rotation rot( makeThreeVector(1.0, 0.0, 0.0), -0.5*S3D::PI );

////////////////////////////////////////////////////////////////////////////////////////////////////

  testass::control::start_section( "Test Camera" );

  S3D::camera_test* the_camera = new S3D::camera_test( makeThreeVector( 0.0, -10.0, 0.0), rot, 20.0, 20.0 );
  the_camera->setPixels( 100, 100 );
  S3D::manager::getInstance()->setCamera( the_camera );

  std::cout << "Camera Configured" << std::endl;

  const S3D::frame* f = S3D::manager::getInstance()->getFrame();

  std::cout << "Built Frame" << std::endl;

  f->dump( std::string("test_data/camera_test_image.bmp") );

  std::cout << "Frame dumped to file" << std::endl;

////////////////////////////////////////////////////////////////////////////////////////////////////

  testass::control::start_section( "Linescan Camera" );

  S3D::camera_lineScan* ls_camera = new S3D::camera_lineScan( makeThreeVector( 0.0, -10.0, 0.0), rot, 20.0, 20.0 );
  S3D::manager::getInstance()->setCamera( ls_camera );

  std::cout << "Camera Configured" << std::endl;

  const S3D::frame* ls_f = S3D::manager::getInstance()->getFrame();

  std::cout << "Built Frame" << std::endl;

//  f->dump( std::string("test_data/camera_test_image.bmp") );
//
//  std::cout << "Frame dumped to file" << std::endl;

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

