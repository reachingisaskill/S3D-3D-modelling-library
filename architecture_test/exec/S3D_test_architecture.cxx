
#include "S3D_manager.h"
#include "S3D_version.h"
#include "S3D_primitives.h"
#include "S3D_materials.h"
#include "S3D_lights.h"
#include "S3D_cameras.h"
#include "S3D_defs.h"

#include "logtastic.h"

#include <iostream>


void addSomeShapes();


int main( int, char** )
{
  logtastic::addLogFile( "./architecture_test.log" );
  logtastic::init( "Testing S3D Manager Functionality", S3D_VERSION_NUMBER );

  S3D::manager::createInstance();

  S3D::manager* man = S3D::manager::getInstance();


  INFO_LOG( "Making basic material" );
  S3D::material_base* mat = (S3D::material_base*) new S3D::material_simple( S3D::colour( 1.0, 0.0, 0.0 ) );
  man->addMaterial( "simple", mat );
  S3D::material_base* world_mat = (S3D::material_base*) new S3D::material_simple( S3D::colour( 0.0, 0.3, 0.8 ) );
  man->addMaterial( "room", world_mat );


  INFO_LOG( "Making world sphere" );
  S3D::object_base* world = (S3D::object_base*) new S3D::sphere( world_mat, 20.0 );
  man->setWorld( world, 0 );

//  INFO_LOG( "Making world box" );
//  S3D::object_base* world = (S3D::object_base*) new S3D::box( world_mat, 20.0, 20.0, 20.0 );
//  man->setWorld( world, 0 ); // Layer zero, visible!


  INFO_LOG( "Making test sphere." );
  S3D::object_base* test_sphere = (S3D::object_base*) new S3D::sphere( mat, 1.0 );
  test_sphere->setPosition( S3D::point( 0.0, 0.0, 0.0 ) );
  man->addObject( test_sphere );

//  INFO_LOG( "Making test box." );
//  S3D::object_base* test_box = (S3D::object_base*) new S3D::box( mat, 2.0, 5.0, 1.0 );
//  test_box->setPosition( S3D::point( 0.0, 0.0, 0.0 ) );
//  test_box->setRotation( S3D::rotation( S3D::unit_threeVector_z, S3D::PI/5.0 ) );
//  test_box->rotate( S3D::rotation( S3D::unit_threeVector_x, S3D::PI/6.0 ) );
//  man->addObject( test_box );


  INFO_LOG( "Added line scan camera." );
  S3D::rotation camera_rot( makeThreeVector(1.0, 0.0, 0.0), -0.5*S3D::PI );
  S3D::point camera_pos( 0.0, -5.0, 0.0 );
  S3D::camera_base* camera = (S3D::camera_base*) new S3D::camera_lineScan( camera_pos, camera_rot, 20.0, 20.0 );
  camera->setPixels( 500, 500 );
  man->setCamera( camera );


  INFO_LOG( "Adding point light source." );
  S3D::light_base* the_light = (S3D::light_base*) new S3D::light_pointSource( S3D::colour( 1.0, 1.0, 1.0 ),  1000.0 );
  the_light->setPosition( S3D::point( 0.1, -5.0, 0.0 ) );
  man->addLight( the_light );

//  S3D::light_base* another_light = (S3D::light_base*) new S3D::light_pointSource( S3D::colour( 1.0, 1.0, 1.0 ),  500.0 );
//  another_light->setPosition( S3D::point( 5.0, -10.0, 5.0 ) );
//  man->addLight( another_light );

  INFO_LOG( "Rendering scene." );
  const S3D::frame* f = man->getFrame();
  f->dump( std::string("camera_test_image.bmp") );


  S3D::manager::killInstance();

  logtastic::stop();
  return 0;
}

