
#include <iostream>
#include <string>

#include "S3D.h"

#include "testass.h"


int main( int, char** )
{
  S3D::manager::createInstance();
  S3D::manager* manager = S3D::manager::getInstance();
  manager->setWorld( new S3D::box( 40, 40, 40, S3D::the_origin ) );

  S3D::sphere* sph1 = new S3D::sphere( 3.0, makeThreeVector( 0.0, 0.0, 0.0 ) );
  S3D::box* box1 = new S3D::box( 1.0, 1.0, 1.0, makeThreeVector( -5.0, 0.0, 0.0 ) );

  S3D::material_simple* sph1_mat = new S3D::material_simple( S3D::colour( 1.0, 0.0, 0.0 ) );
  S3D::material_simple* box1_mat = new S3D::material_simple( S3D::colour( 0.0, 1.0, 0.0 ) );

  sph1->setMaterial( sph1_mat );
  box1->setMaterial( box1_mat );

  manager->add3DObject( (S3D::object_3D_base*)sph1 );
//  manager->add3DObject( (S3D::object_3D_base*)box1 );

  S3D::light_interface* light = new S3D::light_pointSource( makeThreeVector( 0.0, 0.0, 10.0 ), 100.0 );
  manager->addLight( light );

  S3D::rotation rot( makeThreeVector(1.0, 0.0, 0.0), -0.5*S3D::PI );

////////////////////////////////////////////////////////////////////////////////////////////////////

  S3D::camera_lineScan* ls_camera = new S3D::camera_lineScan( makeThreeVector( 0.0, -10.0, 0.0), rot, 20.0, 20.0 );
  ls_camera->setPixels( 500, 500 );
  manager->setCamera( ls_camera );

  std::cout << "Camera Configured" << std::endl;

  const S3D::frame* f_ls = manager->getFrame();

  std::cout << "Built Frame" << std::endl;

  f_ls->dump( std::string("test_data/camera_lineScan_image.bmp") );

  std::cout << "Frame dumped to file" << std::endl;

////////////////////////////////////////////////////////////////////////////////////////////////////

  S3D::manager::killInstance();

  return 0;

}
