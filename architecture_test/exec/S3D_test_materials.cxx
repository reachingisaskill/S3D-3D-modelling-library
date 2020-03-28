
#include "S3D_manager.h"
#include "S3D_version.h"
#include "S3D_primitives.h"
#include "S3D_materials.h"
#include "S3D_lights.h"
#include "S3D_cameras.h"
#include "S3D_defs.h"

#include "logtastic.h"
#include "testass.h"

#include <iostream>
#include <iomanip>
#include <string>


using namespace S3D;


int main( int, char** )
{
  testass::control::init( "S3D", "Testing Material Models" );
  testass::control::get()->setVerbosity( testass::control::verb_short );

  logtastic::addLogFile( "./materials_test.log" );
  logtastic::init( "Testing Material Models", S3D_VERSION_NUMBER );

  S3D::manager::createInstance();

  S3D::manager* man = S3D::manager::getInstance();


  INFO_LOG( "Making a simple asic material" );
  S3D::material_base* world_mat = (S3D::material_base*) new S3D::material_simple( S3D::colour( 0.5, 0.5, 0.5 ) );
  man->addMaterial( "room", world_mat );

  INFO_LOG( "Making world sphere" );
  S3D::object_base* world = (S3D::object_base*) new S3D::sphere( world_mat, 100.0 );
  man->setWorld( world, 1 ); // Don't need to consider it


////////////////////////////////////////////////////////////////////////////////////////////////////

  INFO_LOG( "Making test materials and adding to manager." );

  S3D::material_base* mat = (S3D::material_base*) new S3D::material_simple( S3D::colour( 1.0, 0.0, 0.0 ) );
  man->addMaterial( "simple", mat );

  S3D::material_base* mat0 = (S3D::material_base*) new S3D::material_phong( S3D::colour( 1.0, 0.0, 0.0 ), 0.0, 0.0, 0 );
  man->addMaterial( "mat0", mat0 );

  S3D::material_base* mat1 = (S3D::material_base*) new S3D::material_phong( S3D::colour( 1.0, 0.0, 0.0 ), 0.6, 0.0, 0 );
  man->addMaterial( "mat1", mat1 );

  S3D::material_base* mat2 = (S3D::material_base*) new S3D::material_phong( S3D::colour( 0.0, 1.0, 0.0 ), 0.0, 0.6, 1 );
  man->addMaterial( "mat2", mat2 );

  S3D::material_base* mat3 = (S3D::material_base*) new S3D::material_phong( S3D::colour( 0.0, 0.0, 1.0 ), 0.0, 0.6, 3 );
  man->addMaterial( "mat3", mat3 );

  S3D::material_base* mat4 = (S3D::material_base*) new S3D::material_phong( S3D::colour( 0.5, 0.0, 0.5 ), 0.3, 0.3, 2 );
  man->addMaterial( "mat4", mat4 );


  INFO_LOG( "Making test objects and values" );
  S3D::beam test_beam1( 1.0, 1.0, 1.0 );
  S3D::beam test_beam2( 0.0, 1.0, 0.0 );
  S3D::line test_line1( S3D::point( 0.0, 0.0, 10.0 ), makeThreeVector( 0.0, 0.0, -1.0 ) );
  S3D::point test_point( 0.0, 0.0, 0.0 );
  threeVector test_normal1 = makeThreeVector( 0.0, 0.0, 1.0 ).norm();
  threeVector test_normal2 = makeThreeVector( 0.0, 1.0, 1.0 ).norm();
  threeVector test_normal3 = S3D::rotation( S3D::unit_threeVector_x, 0.1 ) * makeThreeVector( 0.0, 0.0, 1.0 ).norm();

  double diff_factor = std::cos( 0.1 );
  double spec_factor = std::cos( 0.2 );

  S3D::object_base* test_obj1 = (S3D::object_base*) new S3D::square_plane( mat, 1.0, 1.0 );


////////////////////////////////////////////////////////////////////////////////////////////////////

  SECTION( "Phong Reflection Model" );

  man->setAmbientLight( S3D::beam( 1.0, 1.0, 1.0 ) );

  S3D::interaction test_inter1( test_point, &test_line1, test_obj1, test_normal1 );

  beam result = mat0->scatter( makeThreeVector( 0.0, 0.0, -1.0 ), test_beam1, test_inter1 );
  ASSERT_APPROX_EQUAL( result.red(),   1.0 );
  ASSERT_APPROX_EQUAL( result.green(), 0.0 );
  ASSERT_APPROX_EQUAL( result.blue(),  0.0 );

  result = mat1->scatter( makeThreeVector( 0.0, 0.0, -1.0 ), test_beam1, test_inter1 );
  ASSERT_APPROX_EQUAL( result.red(),   1.0 + 0.6 );
  ASSERT_APPROX_EQUAL( result.green(), 0.0 + 0.6 );
  ASSERT_APPROX_EQUAL( result.blue(),  0.0 + 0.6 );

  result = mat2->scatter( makeThreeVector( 0.0, 0.0, -1.0 ), test_beam1, test_inter1 );
  ASSERT_APPROX_EQUAL( result.red(),   0.0 + 0.6 );
  ASSERT_APPROX_EQUAL( result.green(), 1.0 + 0.6 );
  ASSERT_APPROX_EQUAL( result.blue(),  0.0 + 0.6 );

  result = mat4->scatter( makeThreeVector( 0.0, 0.0, -1.0 ), test_beam1, test_inter1 );
  ASSERT_APPROX_EQUAL( result.red(),   0.5 + 0.3 + 0.3 );
  ASSERT_APPROX_EQUAL( result.green(), 0.0 + 0.3 + 0.3 );
  ASSERT_APPROX_EQUAL( result.blue(),  0.5 + 0.3 + 0.3 );


  S3D::interaction test_inter2( test_point, &test_line1, test_obj1, test_normal2 );

  result = mat0->scatter( makeThreeVector( 0.0, 0.0, -1.0 ), test_beam1, test_inter2 );
  ASSERT_APPROX_EQUAL( result.red(),   1.0 );
  ASSERT_APPROX_EQUAL( result.green(), 0.0 );
  ASSERT_APPROX_EQUAL( result.blue(),  0.0 );

  result = mat1->scatter( makeThreeVector( 0.0, 0.0, -1.0 ), test_beam1, test_inter2 );
  ASSERT_APPROX_EQUAL( result.red(),   1.0 + 0.6 / std::sqrt(2.0) );
  ASSERT_APPROX_EQUAL( result.green(), 0.0 + 0.6 / std::sqrt(2.0) );
  ASSERT_APPROX_EQUAL( result.blue(),  0.0 + 0.6 / std::sqrt(2.0) );

  result = mat2->scatter( makeThreeVector( 0.0, 0.0, -1.0 ), test_beam1, test_inter2 );
  ASSERT_APPROX_EQUAL( result.red(),   0.0 + 0.0 );
  ASSERT_APPROX_EQUAL( result.green(), 1.0 + 0.0 );
  ASSERT_APPROX_EQUAL( result.blue(),  0.0 + 0.0 );

  result = mat3->scatter( makeThreeVector( 0.0, 0.0, -1.0 ), test_beam1, test_inter2 );
  ASSERT_APPROX_EQUAL( result.red(),   0.0 + 0.0 );
  ASSERT_APPROX_EQUAL( result.green(), 0.0 + 0.0 );
  ASSERT_APPROX_EQUAL( result.blue(),  1.0 + 0.0 );


  S3D::interaction test_inter3( test_point, &test_line1, test_obj1, test_normal3 );

  result = mat3->scatter( makeThreeVector( 0.0, 0.0, -1.0 ), test_beam1, test_inter3 );
  ASSERT_APPROX_EQUAL( result.red(),   0.0 + 0.6 * std::pow( spec_factor, 3 ) );
  ASSERT_APPROX_EQUAL( result.green(), 0.0 + 0.6 * std::pow( spec_factor, 3 ) );
  ASSERT_APPROX_EQUAL( result.blue(),  1.0 + 0.6 * std::pow( spec_factor, 3 ) );

  result = mat4->scatter( makeThreeVector( 0.0, 0.0, -1.0 ), test_beam1, test_inter3 );
  ASSERT_APPROX_EQUAL( result.red(),   0.5 + 0.3 * diff_factor + 0.3 * std::pow( spec_factor, 2 ) );
  ASSERT_APPROX_EQUAL( result.green(), 0.0 + 0.3 * diff_factor + 0.3 * std::pow( spec_factor, 2 ) );
  ASSERT_APPROX_EQUAL( result.blue(),  0.5 + 0.3 * diff_factor + 0.3 * std::pow( spec_factor, 2 ) );


////////////////////////////////////////////////////////////////////////////////////////////////////


  if ( ! testass::control::summarize() )
  {
    testass::control::printReport( std::cout );
  }

  S3D::manager::killInstance();

  testass::control::kill();
  logtastic::stop();
  return 0;
}





