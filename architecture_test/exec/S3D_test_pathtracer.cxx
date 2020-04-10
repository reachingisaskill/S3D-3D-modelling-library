
#include "S3D_manager.h"
#include "S3D_version.h"
#include "S3D_primitives.h"
#include "S3D_materials.h"
#include "S3D_lights.h"
#include "S3D_cameras.h"
#include "S3D_pathtracer.h"
#include "S3D_defs.h"

#include "logtastic.h"

#include <iostream>


const double light_samples_per_area = 0.0;
const double camera_samples_per_pixel = 10.0;
const double path_kill_prob = 0.1;

int main( int, char** )
{
  logtastic::setLogFileDirectory( "./test_data/" );
  logtastic::addLogFile( "pathtracer_test.log" );
  logtastic::init( "Testing S3D Path Tracer Functionality", S3D_VERSION_NUMBER );

  S3D::manager::createInstance();

  S3D::manager* man = S3D::manager::getInstance();


  INFO_LOG( "Making basic material" );
  S3D::material_base* world_mat = (S3D::material_base*) new S3D::material_lambertian( S3D::spectrum( 0.5, 0.5, 0.5 ) );
  man->addMaterial( "world", world_mat );

  S3D::material_base* light_mat = (S3D::material_base*) new S3D::material_lightsource( S3D::spectrum( 1.0, 1.0, 1.0 ), 10.0 );
  man->addMaterial( "light", light_mat );

  S3D::material_base* blue_light_mat = (S3D::material_base*) new S3D::material_lightsource( S3D::spectrum( 0.0, 0.0, 1.0 ), 10.0 );
  man->addMaterial( "blue_light", blue_light_mat );

  S3D::material_base* sph1_mat = (S3D::material_base*) new S3D::material_lambertian( S3D::spectrum( 0.8, 0.0, 0.0 ) );
  man->addMaterial( "sphere1", sph1_mat );

  S3D::material_base* white_mat = (S3D::material_base*) new S3D::material_lambertian( S3D::spectrum( 1.0, 1.0, 1.0 ) );
  man->addMaterial( "white", white_mat );

  S3D::material_base* red_mat = (S3D::material_base*) new S3D::material_lambertian( S3D::spectrum( 1.0, 0.0, 0.0 ) );
  man->addMaterial( "red", red_mat );

  S3D::material_base* green_mat = (S3D::material_base*) new S3D::material_lambertian( S3D::spectrum( 0.0, 1.0, 0.0 ) );
  man->addMaterial( "green", green_mat );

  S3D::material_base* blue_mat = (S3D::material_base*) new S3D::material_lambertian( S3D::spectrum( 0.0, 0.0, 1.0 ) );
  man->addMaterial( "blue", blue_mat );

  S3D::material_base* glass_mat = (S3D::material_base*) new S3D::material_glass( S3D::spectrum( 0.0, 0.0, 0.0 ) );
  glass_mat->setRefractiveIndex( 1.5 );
  man->addMaterial( "glass", glass_mat );

  S3D::material_base* mirror_mat = (S3D::material_base*) new S3D::material_mirror( S3D::spectrum( 0.0, 0.0, 0.0 ) );
  man->addMaterial( "mirror", mirror_mat );

  man->setAmbientLight( S3D::spectrum( 0.0, 0.0, 0.0 ) );
  man->setLightSampleRate( light_samples_per_area );



  INFO_LOG( "Making world sphere" );
  S3D::object_base* world = (S3D::object_base*) new S3D::sphere( world_mat, 100.0 );
  man->setWorld( world, 1 );



  INFO_LOG( "Making the corner of a room" );
  S3D::object_base* wall1 = (S3D::object_base*) new S3D::square_plane( green_mat, 10.0, 10.0 );
  S3D::rotation wr1( S3D::unit_threeVector_x, S3D::PI/2 );
  wall1->setPosition( S3D::point( 0.0, 5.0, 5.0 ) );
  wall1->setRotation( wr1 );
  wall1->rotateAbout( S3D::rotation( S3D::unit_threeVector_z, S3D::PI/2.0 ), S3D::point( 0.0, 0.0, 5.0 ) );
  man->addObject( wall1 );

  S3D::object_base* wall2 = (S3D::object_base*) new S3D::square_plane( red_mat, 10.0, 10.0 );
  S3D::rotation wr2( S3D::unit_threeVector_x, S3D::PI/2 );
  wall2->setPosition( S3D::point( 0.0, 5.0, 5.0 ) );
  wall2->setRotation( wr2 );
  wall2->rotateAbout( S3D::rotation( S3D::unit_threeVector_z, -S3D::PI/2.0 ), S3D::point( 0.0, 0.0, 5.0 ) );
  man->addObject( wall2 );

  S3D::object_base* wall3 = (S3D::object_base*) new S3D::square_plane( white_mat, 10.0, 10.0 );
  S3D::rotation wr3( S3D::unit_threeVector_x, S3D::PI/2 );
  wall3->setPosition( S3D::point( 0.0, 5.0, 5.0 ) );
  wall3->setRotation( wr3 );
  man->addObject( wall3 );

  S3D::object_base* floor = (S3D::object_base*) new S3D::square_plane( white_mat, 10.0, 10.0 );
  S3D::rotation fr;
  floor->setPosition( S3D::point( 0.0, 0.0, 0.0 ) );
  floor->setRotation( fr );
  man->addObject( floor );

  S3D::object_base* ceiling = (S3D::object_base*) new S3D::square_plane( white_mat, 10.0, 10.0 );
  S3D::rotation cr( S3D::unit_threeVector_x, S3D::PI );
  ceiling->setPosition( S3D::point( 0.0, 0.0, 10.0 ) );
  ceiling->setRotation( cr );
  man->addObject( ceiling );

//  S3D::object_base* shade = (S3D::object_base*) new S3D::square_plane( white_mat, 2.0, 2.0 );
//  S3D::rotation shade_r( S3D::unit_threeVector_x, S3D::PI/2.0 );
//  shade->setPosition( S3D::point( 0.0, -2.0, 7.5 ) );
//  shade->setRotation( shade_r );
//  man->addObject( shade );

  S3D::object_base* front = (S3D::object_base*) new S3D::square_plane( white_mat, 10.0, 10.0 );
  S3D::rotation front_r( S3D::unit_threeVector_x, -S3D::PI/2.0 );
  front->setPosition( S3D::point( 0.0, -5.0, 5.0 ) );
  front->setRotation( front_r );
  man->addObject( front );



  INFO_LOG( "Making test sphere 1." );
  S3D::object_base* test_sphere1 = (S3D::object_base*) new S3D::sphere( glass_mat, 1.0 );
  test_sphere1->setPosition( S3D::point( -1.0, -3.0, 1.0 ) );
  man->addObject( test_sphere1 );

  INFO_LOG( "Making test sphere 2." );
  S3D::object_base* test_sphere2 = (S3D::object_base*) new S3D::sphere( white_mat, 1.0 );
  test_sphere2->setPosition( S3D::point( -3.5, -2.0, 1.0 ) );
  man->addObject( test_sphere2 );

  INFO_LOG( "Making test sphere 3." );
  S3D::object_base* test_sphere3 = (S3D::object_base*) new S3D::sphere( blue_mat, 1.0 );
  test_sphere3->setPosition( S3D::point( -1.0, 2.0, 1.0 ) );
  man->addObject( test_sphere3 );

  INFO_LOG( "Making test box." );
  S3D::object_base* test_box = (S3D::object_base*) new S3D::box( white_mat, 3.0, 1.0, 5.0 );
  test_box->setPosition( S3D::point( 3.0, -1.0, 2.5 ) );
  test_box->setRotation( S3D::rotation( S3D::unit_threeVector_z, S3D::PI/4.0 ) );
  man->addObject( test_box );



  INFO_LOG( "Adding light sources." );
  S3D::circular_plane* the_light = new S3D::circular_plane( light_mat, 1.0 );
  the_light->setPosition( S3D::point( 0.0, 0.0, 8.0 ) );
  the_light->setRotation( S3D::rotation( S3D::unit_threeVector_x, S3D::PI ) );
  man->addObject( (S3D::object_base*)the_light );

//  S3D::circular_plane* another_light = new S3D::circular_plane( light_mat, 0.2 );
//  another_light->setPosition( S3D::point( -6.0, -5.0, 5.0 ) );
//  another_light->setRotation( S3D::rotation( S3D::unit_threeVector_z, -S3D::PI/4 ) * S3D::rotation( S3D::unit_threeVector_x, -S3D::PI/2.0 ) );
//  man->addObject( (S3D::object_base*)another_light );

//  S3D::light_base* another_light = (S3D::light_base*) new S3D::light_pointSource( S3D::spectrum( 0.1, 0.1, 1.0 ),  2000.0 );
//  man->addLight( another_light );



//  INFO_LOG( "Adding pinhole camera." );
//  S3D::tracer_pathtracer* tracer = new S3D::tracer_pathtracer();
//  tracer->setMaxDepth( 100 );
//  tracer->setKillProb( path_kill_prob );
//  S3D::camera_base* camera = (S3D::camera_base*) new S3D::camera_sampledPinhole( tracer, S3D::degreesToRadians( 90.0 ), camera_samples_per_pixel );
//  camera->setPosition( S3D::point( 0.0, -15.0, 5.0 ) );
//  camera->setRotation( S3D::rotation( S3D::unit_threeVector_x, -0.5*S3D::PI ) );
//  camera->setPixels( 500, 500 );
//  man->setCamera( camera );

  INFO_LOG( "Adding perspective camera." );
  S3D::tracer_pathtracer* tracer = new S3D::tracer_pathtracer();
  tracer->setMaxDepth( 100 );
  tracer->setKillProb( path_kill_prob );
  S3D::camera_perspective* camera = new S3D::camera_perspective( tracer, S3D::degreesToRadians( 10 ), 10.0, 10.0 );
  camera->setSampleRate( camera_samples_per_pixel );
  camera->setPosition( S3D::point( 0.0, -4.99999, 5.0 ) );
  camera->setRotation( S3D::rotation( S3D::unit_threeVector_x, -0.5*S3D::PI ) );
  camera->setPixels( 500, 500 );
  man->setCamera( (S3D::camera_base*)camera );



  INFO_LOG( "Rendering scene." );
  const S3D::frame* f = man->getFrame();
  f->dump( std::string("pathtracing_test_image.bmp") );

//  INFO_LOG( "Tracing a ray" );
//  tracer->setup();
//  for ( int i = 0; i < 100; ++i )
//  {
//    tracer->traceRay( S3D::point( 0.0, -4.99999, 5.0 ), (S3D::point( -1.0, -3.0, 1.0 ) - S3D::point( 0.0, -4.99999, 5.0  ) ).norm() );
//  }



  S3D::manager::killInstance();

  logtastic::stop();
  return 0;
}

