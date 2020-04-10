
#include "S3D_manager.h"
#include "S3D_version.h"
#include "S3D_primitives.h"
#include "S3D_materials.h"
#include "S3D_lights.h"
#include "S3D_cameras.h"
#include "S3D_bdpt.h"
#include "S3D_defs.h"

#include "logtastic.h"

#include <iostream>


const double light_samples_per_area = 0.0;
const double camera_samples_per_pixel = 10.0;

int main( int, char** )
{
  logtastic::setLogFileDirectory( "./test_data/" );
  logtastic::addLogFile( "bdpt_test.log" );
  logtastic::init( "Testing S3D Bidirectional Path Tracer Functionality", S3D_VERSION_NUMBER );

  S3D::manager::createInstance();

  S3D::manager* man = S3D::manager::getInstance();


  INFO_LOG( "Making basic material" );
  S3D::material_base* world_mat = (S3D::material_base*) new S3D::material_lambertian( S3D::spectrum( 0.5, 0.5, 0.5 ) );
  man->addMaterial( "world", world_mat );

  S3D::material_base* room_mat = (S3D::material_base*) new S3D::material_lambertian( S3D::spectrum( 0.7, 0.7, 0.7 ) );
  man->addMaterial( "room", room_mat );

  S3D::material_base* floor_mat = (S3D::material_base*) new S3D::material_lambertian( S3D::spectrum( 0.0, 0.7, 0.0 ) );
  man->addMaterial( "floor", floor_mat );

  S3D::material_base* light_mat = (S3D::material_base*) new S3D::material_lightsource( S3D::spectrum( 1.0, 1.0, 1.0 ), 1.0 );
  man->addMaterial( "light", light_mat );

  S3D::material_base* sph1_mat = (S3D::material_base*) new S3D::material_lambertian( S3D::spectrum( 0.8, 0.0, 0.0 ) );
  man->addMaterial( "sphere1", sph1_mat );

  S3D::material_base* glass_mat = (S3D::material_base*) new S3D::material_glass( S3D::spectrum( 0.0, 0.0, 0.0 ) );
  man->addMaterial( "glass", glass_mat );

  S3D::material_base* mirror_mat = (S3D::material_base*) new S3D::material_mirror( S3D::spectrum( 0.0, 0.0, 0.0 ) );
  man->addMaterial( "mirror", mirror_mat );

  S3D::material_base* box_mat = (S3D::material_base*) new S3D::material_lambertian( S3D::spectrum( 0.0, 0.0, 0.5 ) );
  man->addMaterial( "box", box_mat );

  man->setAmbientLight( S3D::spectrum( 0.0, 0.0, 0.0 ) );
  man->setLightSampleRate( light_samples_per_area );



  INFO_LOG( "Making world sphere" );
  S3D::object_base* world = (S3D::object_base*) new S3D::sphere( world_mat, 100.0 );
  man->setWorld( world, 1 );


  INFO_LOG( "Making the corner of a room" );
  S3D::object_base* wall1 = (S3D::object_base*) new S3D::square_plane( room_mat, 10.0, 10.0 );
  S3D::rotation wr1( S3D::unit_threeVector_x, S3D::PI/2 );
  wall1->setPosition( S3D::point( 0.0, 5.0, 5.0 ) );
  wall1->setRotation( wr1 );
  wall1->rotateAbout( S3D::rotation( S3D::unit_threeVector_z, S3D::PI/2.0 ), S3D::point( 0.0, 0.0, 5.0 ) );
  man->addObject( wall1 );

  S3D::object_base* wall2 = (S3D::object_base*) new S3D::square_plane( room_mat, 10.0, 10.0 );
  S3D::rotation wr2( S3D::unit_threeVector_x, S3D::PI/2 );
  wall2->setPosition( S3D::point( 0.0, 5.0, 5.0 ) );
  wall2->setRotation( wr2 );
  wall2->rotateAbout( S3D::rotation( S3D::unit_threeVector_z, -S3D::PI/2.0 ), S3D::point( 0.0, 0.0, 5.0 ) );
  man->addObject( wall2 );

  S3D::object_base* wall3 = (S3D::object_base*) new S3D::square_plane( room_mat, 10.0, 10.0 );
  S3D::rotation wr3( S3D::unit_threeVector_x, S3D::PI/2 );
  wall3->setPosition( S3D::point( 0.0, 5.0, 5.0 ) );
  wall3->setRotation( wr3 );
  man->addObject( wall3 );

  S3D::object_base* floor = (S3D::object_base*) new S3D::square_plane( floor_mat, 10.0, 10.0 );
  S3D::rotation fr;
  floor->setPosition( S3D::point( 0.0, 0.0, 0.0 ) );
  floor->setRotation( fr );
  man->addObject( floor );


  INFO_LOG( "Making test sphere 1." );
  S3D::object_base* test_sphere1 = (S3D::object_base*) new S3D::sphere( sph1_mat, 1.0 );
  test_sphere1->setPosition( S3D::point( 2.0, -2.0, 1.0 ) );
  man->addObject( test_sphere1 );

//  INFO_LOG( "Making test sphere 2." );
//  S3D::object_base* test_sphere2 = (S3D::object_base*) new S3D::sphere( glass_mat, 1.0 );
//  test_sphere2->setPosition( S3D::point( -2.0, -2.0, 1.0 ) );
//  man->addObject( test_sphere2 );
//
//  INFO_LOG( "Making test sphere 3." );
//  S3D::object_base* test_sphere3 = (S3D::object_base*) new S3D::sphere( mirror_mat, 1.0 );
//  test_sphere3->setPosition( S3D::point( 0.0, 2.0, 1.0 ) );
//  man->addObject( test_sphere3 );

//  INFO_LOG( "Making test box." );
//  S3D::object_base* test_box = (S3D::object_base*) new S3D::box( box_mat, 2.0, 1.0, 5.0 );
//  test_box->setPosition( S3D::point( -1.0, 1.0, 2.5 ) );
//  test_box->setRotation( S3D::rotation( S3D::unit_threeVector_z, S3D::PI/5.0 ) );
//  man->addObject( test_box );


  INFO_LOG( "Adding pinhole camera." );
  S3D::tracer_bdpt* tracer = new S3D::tracer_bdpt();
  tracer->setKillProb( 0.2 );
  S3D::camera_base* camera = (S3D::camera_base*) new S3D::camera_sampledPinhole( tracer, S3D::degreesToRadians( 90.0 ), camera_samples_per_pixel );
  camera->setPosition( S3D::point( 0.0, -15.0, 5.0 ) );
  camera->setRotation( S3D::rotation( S3D::unit_threeVector_x, -0.5*S3D::PI ) );
  camera->setPixels( 500, 500 );
  man->setCamera( camera );


  INFO_LOG( "Adding light sources." );

  S3D::circular_plane* the_light = new S3D::circular_plane( light_mat, 1.0 );
  the_light->setPosition( S3D::point( 0.0, 0.0, 8.0 ) );
  the_light->setRotation( S3D::rotation( S3D::unit_threeVector_x, S3D::PI ) );
  man->addObject( (S3D::object_base*)the_light );

//  S3D::circular_plane* another_light = new S3D::circular_plane( light_mat, 1.0 );
//  another_light->setPosition( S3D::point( -5.0, -5.0, 3.0 ) );
//  another_light->setRotation( S3D::rotation( S3D::unit_threeVector_z, -S3D::PI/4 ) * S3D::rotation( S3D::unit_threeVector_x, -S3D::PI/2.0 ) );
//  man->addObject( (S3D::object_base*)another_light );

//  S3D::light_base* another_light = (S3D::light_base*) new S3D::light_pointSource( S3D::spectrum( 0.1, 0.1, 1.0 ),  2000.0 );
//  man->addLight( another_light );

  INFO_LOG( "Rendering scene." );
  const S3D::frame* f = man->getFrame();
  f->dump( std::string("bdpt_test_image.bmp") );

//  INFO_LOG( "Tracing a ray" );
//  tracer->setup();
//  for ( int i = 0; i < 100; ++i )
//  {
//    tracer->traceRay( S3D::point( 0.0, -15.0, 5.0 ), (S3D::point( -2.0, -2.0, 1.0 ) - S3D::point( 0.0, -15.0, 5.0 ) ).norm() );
//  }



  S3D::manager::killInstance();

  logtastic::stop();
  return 0;
}

