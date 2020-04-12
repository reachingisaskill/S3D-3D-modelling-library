
#include "S3D_manager.h"
#include "S3D_version.h"
#include "S3D_primitives.h"
#include "S3D_materials.h"
#include "S3D_lights.h"
#include "S3D_cameras.h"
#include "S3D_recursiveraytracer.h"
#include "S3D_pathtracer.h"
#include "S3D_random.h"
#include "S3D_defs.h"

#include "logtastic.h"
#include "testass.h"

#include <iostream>
#include <iomanip>
#include <string>


using namespace S3D;


int main( int, char** )
{
  testass::control::init( "S3D", "Testing The Basic Algorithms" );
  testass::control::get()->setVerbosity( testass::control::verb_short );

  logtastic::setLogFileDirectory( "./test_data/" );
  logtastic::addLogFile( "./algorithms_test.log" );
  logtastic::setPrintToScreenLimit( logtastic::error );
  logtastic::init( "Testing S3D Algorithms", S3D_VERSION_NUMBER );

  S3D::manager::createInstance();

  S3D::manager* man = S3D::manager::getInstance();


  INFO_LOG( "Making basic material" );
  S3D::material_base* mat = (S3D::material_base*) new S3D::material_simple( S3D::spectrum( 1.0, 0.0, 0.0 ) );
  man->addMaterial( "simple", mat );
  S3D::material_base* world_mat = (S3D::material_base*) new S3D::material_simple( S3D::spectrum( 0.0, 0.3, 0.8 ) );
  man->addMaterial( "room", world_mat );


  INFO_LOG( "Making world sphere" );
  S3D::object_base* world = (S3D::object_base*) new S3D::sphere( mat, 100.0 );
  man->setWorld( world, 0 );


  double sphere_radius = 5.0;
  double albedo = 0.3;
  double emittance = 5.0;

  material_base* glowing = new material_glowing( S3D::spectrum( 1.0, 1.0, 1.0 ), S3D::spectrum( albedo, albedo, albedo ), emittance );
  sphere* glowing_sphere = new sphere( glowing, sphere_radius );
  glowing_sphere->setPosition( point( 10.0, 0.0, 0.0 ) );
  man->addObject( (object_base*)glowing_sphere );

//  tracer_pathtracer pt;
//  pt.setLightSampleRate( 0.0 );
//  pt.setMaxDepth( 2 );
//  pt.setKillProb( 0.0 );
//  pt.setup();


  point p1 = glowing_sphere->getPosition() + makeThreeVector( sphere_radius, 0.0, 0.0 );

  double surface_area = 4.0*PI*sphere_radius*sphere_radius;
  unsigned int n_samples = 100000;

  random::reset();


  INFO_STREAM << "Analytic solution to the complete sum = " << emittance / ( 1.0 - albedo );
  threeVector normal = makeThreeVector( -sphere_radius, 0.0, 0.0 ).norm();

//  double sum = 0.0;
//  for ( unsigned int i = 0; i < n_samples; ++i )
//  {
//    threeVector direction = random::uniformHemisphere( normal );
//    double cos_theta = std::fabs(direction * normal);
//
////    double theta = random::uniformDouble() * PI / 2.0;
////    double phi = random::uniformDouble() * PI * 2.0;
//
//    sum += cos_theta*cos_theta / delta_p1.square();
//
//  }
//
//  INFO_STREAM << "Integral[ Cos_theta ] over hemisphere = " << 2.0*PI * sum / n_samples;


//  double sum = 0.0;
//  for ( unsigned int i = 0; i < n_samples; ++i )
//  {
//    for ( unsigned int i = 0; i < n_samples; ++i )
//    {
//      threeVector direction = random::uniformHemisphere( normal );
//      double cos_theta = std::fabs(direction * normal);
//
//  //    double theta = random::uniformDouble() * PI / 2.0;
//  //    double phi = random::uniformDouble() * PI * 2.0;
//
//      sum += cos_theta*cos_theta / delta_p1.square();
//
//    }
//  }
//
//  INFO_STREAM << "Integral[ Cos_theta ] over hemisphere = " << 2.0*PI * sum / n_samples;



//  n_samples = 10000000;
//  double phasespace_volume = 0.0;
//  for ( unsigned int i = 0; i < n_samples; ++i )
//  {
//    surfacemap p2 = glowing_sphere->sampleSurface();
//    threeVector delta_p1 = p2.getPosition() - p1;
//    double cos_theta1 = std::fabs( p2.getNormal() * delta_p1.norm() );
//
//    phasespace_volume += surface_area  * cos_theta1 / ( delta_p1.square() * n_samples );
//  }
//  INFO_STREAM << "Path Integration Model Volume : " << phasespace_volume;



  n_samples = 100;

  double sum = 0.0;
  double sum1 = 0.0;
  double sum2 = 0.0;
  for ( unsigned int i = 0; i < n_samples; ++i )
  {
    surfacemap p2 = glowing_sphere->sampleSurface();
    threeVector delta_p1 = p2.getPosition() - p1;
    double cos_theta1 = std::fabs( p2.getNormal() * delta_p1.norm() );

    sum += surface_area*(albedo/PI) * emittance * cos_theta1*cos_theta1 / ( delta_p1.square() );

    for ( unsigned int j = 0; j < n_samples; ++j )
    {
      surfacemap p3 = glowing_sphere->sampleSurface();
      threeVector delta_p2 = p3.getPosition() - p2.getPosition();
      double cos_theta2 = p3.getNormal() * delta_p2.norm();

      sum1 += surface_area* emittance * ( (albedo/PI)*cos_theta2*cos_theta2/delta_p2.square() ) * ( (albedo/PI)*cos_theta1*cos_theta1/delta_p1.square() );

      for ( unsigned int k = 0; k < n_samples; ++k )
      {
        surfacemap p4 = glowing_sphere->sampleSurface();
        threeVector delta_p3 = p4.getPosition() - p3.getPosition();
        double cos_theta3 = p4.getNormal() * delta_p3.norm();

        sum2 += surface_area* emittance * ( (albedo/PI)*cos_theta3*cos_theta3/delta_p3.square() ) * ( (albedo/PI)*cos_theta2*cos_theta2/delta_p2.square() ) * ( (albedo/PI)*cos_theta1*cos_theta1/delta_p1.square() );
      }

    }
  }
  INFO_STREAM << "Path Integration Model Predicted n = 1 : " << emittance;
  INFO_STREAM << "Path Integration Model Predicted n = 2 : " << emittance + (sum / n_samples);
  INFO_STREAM << "Path Integration Model Predicted n = 3 : " << emittance + (sum / n_samples) + (sum1 / (n_samples*n_samples));
  INFO_STREAM << "Path Integration Model Predicted n = 4 : " << emittance + (sum / n_samples) + (sum1 / (n_samples*n_samples)) + (sum2 / (n_samples*n_samples*n_samples));





  sum = 0.0;
  sum1 = 0.0;
  sum2 = 0.0;
  for ( unsigned int i = 0; i < n_samples; ++i )
  {
    threeVector direction = random::uniformHemisphere( normal );
    double cos_theta1 = std::fabs(direction * normal);

    sum += (albedo/PI) * emittance * cos_theta1;

    for ( unsigned int j = 0; j < n_samples; ++j )
    {
      threeVector direction1 = random::uniformHemisphere( normal );
      double cos_theta2 = std::fabs(direction1 * normal);

      sum1 += emittance*(albedo/PI)*cos_theta2 * (albedo/PI)*cos_theta1;

      for ( unsigned int k = 0; k < n_samples; ++k )
      {
        threeVector direction2 = random::uniformHemisphere( normal );
        double cos_theta3 = std::fabs(direction2 * normal);

        sum2 += emittance*(albedo/PI)*cos_theta3 * (albedo/PI)*cos_theta2 * (albedo/PI)*cos_theta1;
      }
    }
  }
  INFO_STREAM << "Path Tracing Model Predicted n = 1 : " << emittance;
  INFO_STREAM << "Path Tracing Model Predicted n = 2 : " << emittance + 2.0*PI*(sum / n_samples);
  INFO_STREAM << "Path Tracing Model Predicted n = 3 : " << emittance + 2.0*PI*(sum / n_samples) + 4.0*PI*PI*(sum1 / (n_samples*n_samples));
  INFO_STREAM << "Path Tracing Model Predicted n = 4 : " << emittance + 2.0*PI*(sum / n_samples) + 4.0*PI*PI*(sum1 / (n_samples*n_samples)) + 8.0*PI*PI*PI*(sum2 / (n_samples*n_samples*n_samples) );
  




  n_samples = 100000;

  sum = 0.0;
  sum1 = 0.0;
  sum2 = 0.0;
  for ( unsigned int i = 0; i < n_samples; ++i )
  {
    threeVector direction = random::uniformHemisphere( normal );
    double cos_theta1 = std::fabs(direction * normal);

    threeVector direction1 = random::uniformHemisphere( normal );
    double cos_theta2 = std::fabs(direction1 * normal);

    threeVector direction2 = random::uniformHemisphere( normal );
    double cos_theta3 = std::fabs(direction2 * normal);

    double v3 = emittance;

    double v2 = emittance + 2.0*PI*v3*(albedo/PI)*cos_theta3;

    double v1 = emittance + 2.0*PI*v2*(albedo/PI)*cos_theta2;

    sum += v3 * 2.0*PI*(albedo/PI)*cos_theta1;

    sum1 += v2 * 2.0*PI*(albedo/PI)*cos_theta1;

    sum2 += v1 * 2.0*PI*(albedo/PI)*cos_theta1;

  }
  INFO_STREAM << "Path Tracing 2 Predicted n = 1 : " << emittance;
  INFO_STREAM << "Path Tracing 2 Predicted n = 2 : " << emittance + (sum / n_samples);
  INFO_STREAM << "Path Tracing 2 Predicted n = 3 : " << emittance + (sum1 / n_samples);
  INFO_STREAM << "Path Tracing 2 Predicted n = 4 : " << emittance + (sum2 / n_samples);
  





  n_samples = 10000;
  unsigned int n_vertices = 1000000;
  double kill = 0.001;

  sum = 0.0;
  sum1 = 0.0;
  sum2 = 0.0;
  for ( unsigned int i = 0; i < n_samples; ++i )
  {
    double path_sum = 0.0;
    double scale = 1.0;
    scale = 1.0 / ( 1.0 - kill );

    while( random::uniformDouble() >= kill )
    {
//      double test = random::uniformDouble();
//      if ( test < kill ) break;

      threeVector direction = random::uniformHemisphere( normal );
      double cos_theta = std::fabs(direction * normal);

      path_sum = scale*(emittance + 2.0*PI*path_sum*(albedo/PI)*cos_theta);
    }

    threeVector direction = random::uniformHemisphere( normal );
    double cos_theta = std::fabs(direction * normal);

    sum += emittance + 2.0*PI*path_sum*(albedo/PI)*cos_theta;
  }
  INFO_STREAM << "Path Tracing 3 Predicted : " << (sum / n_samples);



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


