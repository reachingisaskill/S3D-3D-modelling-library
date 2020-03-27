
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
  S3D::material_base* world_mat = (S3D::material_base*) new S3D::material_phong( S3D::colour( 0.5, 0.5, 0.5 ), 1.0, 0.0, 0 );
  man->addMaterial( "world", world_mat );

  S3D::material_base* room_mat = (S3D::material_base*) new S3D::material_phong( S3D::colour( 0.5, 0.5, 0.5 ), 0.7, 0.3, 1 );
  man->addMaterial( "room", room_mat );

  S3D::material_base* sph_mat = (S3D::material_base*) new S3D::material_phong( S3D::colour( 1.0, 0.0, 0.0 ), 0.4, 0.5, 2 );
  man->addMaterial( "sphere", sph_mat );

  S3D::material_base* box_mat = (S3D::material_base*) new S3D::material_phong( S3D::colour( 0.0, 0.0, 1.0 ), 0.4, 0.5, 2 );
  man->addMaterial( "box", box_mat );

  S3D::material_phong::setAmbientLight( S3D::beam( 0.3, 0.3, 0.3 ) );



  INFO_LOG( "Making world sphere" );
  S3D::object_base* world = (S3D::object_base*) new S3D::sphere( world_mat, 100.0 );
  man->setWorld( world, 1 );


  INFO_LOG( "Making the corner of a room" );
  S3D::object_base* wall1 = (S3D::object_base*) new S3D::square_plane( room_mat, 10.0, 10.0 );
  S3D::rotation wr1( S3D::unit_threeVector_x, S3D::PI/2 );
  wall1->setPosition( S3D::point( 0.0, 5.0, 5.0 ) );
  wall1->setRotation( wr1 );
  wall1->rotateAbout( S3D::rotation( S3D::unit_threeVector_z, S3D::PI/4.0 ), S3D::point( 0.0, 0.0, 5.0 ) );
  man->addObject( wall1 );

  S3D::object_base* wall2 = (S3D::object_base*) new S3D::square_plane( room_mat, 10.0, 10.0 );
  S3D::rotation wr2( S3D::unit_threeVector_x, S3D::PI/2 );
  wall2->setPosition( S3D::point( 0.0, 5.0, 5.0 ) );
  wall2->setRotation( wr2 );
  wall2->rotateAbout( S3D::rotation( S3D::unit_threeVector_z, -S3D::PI/4.0 ), S3D::point( 0.0, 0.0, 5.0 ) );
  man->addObject( wall2 );

  S3D::object_base* wall3 = (S3D::object_base*) new S3D::square_plane( room_mat, 10.0, 10.0 );
  S3D::rotation wr3( S3D::unit_threeVector_z, -S3D::PI/4 );
  wall3->setPosition( S3D::point( 0.0, 0.0, 0.0 ) );
  wall3->setRotation( wr3 );
  man->addObject( wall3 );


  INFO_LOG( "Making test sphere." );
  S3D::object_base* test_sphere = (S3D::object_base*) new S3D::sphere( sph_mat, 1.0 );
  test_sphere->setPosition( S3D::point( 1.0, 0.0, 1.0 ) );
  man->addObject( test_sphere );

  INFO_LOG( "Making test box." );
  S3D::object_base* test_box = (S3D::object_base*) new S3D::box( box_mat, 2.0, 1.0, 5.0 );
  test_box->setPosition( S3D::point( -2.0, -1.0, 2.5 ) );
  test_box->setRotation( S3D::rotation( S3D::unit_threeVector_z, S3D::PI/5.0 ) );
  man->addObject( test_box );


//  INFO_LOG( "Added line scan camera." );
//  S3D::rotation camera_rot( makeThreeVector(1.0, 0.0, 0.0), -0.5*S3D::PI );
//  S3D::point camera_pos( 0.0, -5.0, 2.0 );
//  S3D::camera_base* camera = (S3D::camera_base*) new S3D::camera_lineScan( camera_pos, camera_rot, 20.0, 20.0 );
//  camera->setPixels( 500, 500 );
//  man->setCamera( camera );


  INFO_LOG( "Added pinhole camera." );
  S3D::rotation camera_rot = S3D::rotation( S3D::unit_threeVector_x, -S3D::PI/8 ) * S3D::rotation( S3D::unit_threeVector_x, -0.5*S3D::PI );
  S3D::point camera_pos( 0.0, -15.0, 10.0 );
  S3D::camera_base* camera = (S3D::camera_base*) new S3D::camera_pinhole( camera_pos, camera_rot, S3D::degreesToRadians( 90.0 ) );
  camera->setPixels( 500, 500 );
  man->setCamera( camera );


  INFO_LOG( "Adding point light source." );
//  S3D::light_base* the_light = (S3D::light_base*) new S3D::light_pointSource( S3D::colour( 1.0, 1.0, 1.0 ),  1000.0 );
//  the_light->setPosition( S3D::point( 1.0, -5.0, 20.0 ) );
//  man->addLight( the_light );

  S3D::light_spotlight* the_light = new S3D::light_spotlight( S3D::colour( 1.0, 1.0, 1.0 ),  1.0, 1.0 );
  the_light->setPosition( S3D::point( 1.0, -5.0, 20.0 ) );
  the_light->setRotation( S3D::rotation( S3D::unit_threeVector_x, S3D::PI ) );
  the_light->setNumSamples( 10 );
  man->addLight( (S3D::light_base*)the_light );

  S3D::light_base* another_light = (S3D::light_base*) new S3D::light_pointSource( S3D::colour( 0.1, 0.1, 1.0 ),  2000.0 );
  another_light->setPosition( S3D::point( -2.0, -5.0, 5.0 ) );
  man->addLight( another_light );

  INFO_LOG( "Rendering scene." );
  const S3D::frame* f = man->getFrame();
  f->dump( std::string("camera_test_image.bmp") );


  S3D::manager::killInstance();

  logtastic::stop();
  return 0;
}

