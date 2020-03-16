
#include <iostream>
#include <string>

#include "S3D.h"

#include "testass.h"


int main( int, char** )
{

  testass::control::init( "S3D", "Line Scan Rendering" );
//  testass::control::get()->setVerbosity( testass::controll::verb_short );

  S3D::manager::createInstance();
  S3D::manager* manager = S3D::manager::getInstance();
  manager->setWorld( new S3D::box( 40, 40, 40, S3D::the_origin ) );

  S3D::sphere* sph1 = new S3D::sphere( 3.0, makeThreeVector( 2.0, 5.0, 2.0 ) );
  S3D::sphere* sph2 = new S3D::sphere( 0.5, makeThreeVector( 0.0, 0.0, 0.0 ) );
  S3D::box* box1 = new S3D::box( 1.0, 1.0, 1.0, makeThreeVector( -5.0, 0.0, 0.0 ) );

  sph1->setColour( S3D::colour( 1.0, 0.0, 0.0 ) );
  sph2->setColour( S3D::colour( 0.0, 0.0, 1.0 ) );
  box1->setColour( S3D::colour( 0.0, 1.0, 0.0 ) );

  manager->add3DObject( (S3D::object_3D_base*)sph1 );
  manager->add3DObject( (S3D::object_3D_base*)sph2 );
  manager->add3DObject( (S3D::object_3D_base*)box1 );

  S3D::rotation rot( makeThreeVector(1.0, 0.0, 0.0), -0.5*S3D::PI );

////////////////////////////////////////////////////////////////////////////////////////////////////

  testass::control::start_section( "Test Camera" );

  S3D::camera_test* the_camera = new S3D::camera_test( makeThreeVector( 0.0, -10.0, 0.0), rot, 20.0, 20.0 );
  the_camera->setPixels( 100, 100 );
  manager->setCamera( the_camera );

  std::cout << "Camera Configured" << std::endl;

  const S3D::frame* f = manager->getFrame();

  std::cout << "Built Frame" << std::endl;

  f->dump( std::string("test_data/camera_test_image.bmp") );

  std::cout << "Frame dumped to file" << std::endl;

////////////////////////////////////////////////////////////////////////////////////////////////////

  testass::control::start_section( "Linescan Camera" );

  S3D::camera_lineScan* ls_camera = new S3D::camera_lineScan( makeThreeVector( 0.0, -10.0, 0.0), rot, 20.0, 20.0 );
  ls_camera->setPixels( 500, 500 );
  manager->setCamera( ls_camera );

  std::cout << "Camera Configured" << std::endl;

  const S3D::frame* f_ls = manager->getFrame();

  std::cout << "Built Frame" << std::endl;

  f_ls->dump( std::string("test_data/camera_lineScan_image.bmp") );

  std::cout << "Frame dumped to file" << std::endl;

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

