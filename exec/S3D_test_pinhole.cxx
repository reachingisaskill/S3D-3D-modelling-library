
#include <iostream>
#include <string>

#include "S3D.h"

#include "testass.h"


int main( int, char** )
{
  S3D::manager::createInstance();
  S3D::manager* manager = S3D::manager::getInstance();
  manager->setWorld( new S3D::box( 40, 40, 40, S3D::the_origin ) );

  S3D::sphere* sph1 = new S3D::sphere( 3.0, makeThreeVector( 2.0, 5.0, 2.0 ) );
  S3D::sphere* sph2 = new S3D::sphere( 0.5, makeThreeVector( 0.0, 0.0, 0.0 ) );
  S3D::box* box1 = new S3D::box( 1.0, 1.0, 1.0, makeThreeVector( -5.0, 0.0, 0.0 ) );
  S3D::box* box2 = new S3D::box( 1.0, 1.0, 1.0, makeThreeVector( -5.0, 0.0, 5.0 ) );

  sph1->setColour( S3D::colour( 1.0, 0.0, 0.0 ) );
  sph2->setColour( S3D::colour( 0.0, 0.0, 1.0 ) );
  box1->setColour( S3D::colour( 0.0, 1.0, 0.0 ) );
  box2->setColour( S3D::colour( 0.0, 1.0, 0.0 ) );

  manager->add3DObject( (S3D::object_3D_base*)sph1 );
  manager->add3DObject( (S3D::object_3D_base*)sph2 );
  manager->add3DObject( (S3D::object_3D_base*)box1 );
  manager->add3DObject( (S3D::object_3D_base*)box2 );

  S3D::rotation rot( makeThreeVector(1.0, 0.0, 0.0), -0.5*S3D::PI );

////////////////////////////////////////////////////////////////////////////////////////////////////

  S3D::camera_pinhole* the_camera = new S3D::camera_pinhole( makeThreeVector( 0.0, -10.0, 0.0), rot, 2.0*S3D::PI/3.0 );
  the_camera->setPixels( 500, 500 );
  manager->setCamera( the_camera );

  std::cout << "Camera Configured" << std::endl;

  const S3D::frame* f_ls = manager->getFrame();

  std::cout << "Built Frame" << std::endl;

  f_ls->dump( std::string("test_data/camera_pinhole_image.bmp") );

  std::cout << "Frame dumped to file" << std::endl;

////////////////////////////////////////////////////////////////////////////////////////////////////

  S3D::manager::killInstance();

  std::cout << std::endl;

  return 0;

}

