
#include "logtastic.h"
#include "S3D.h"

int main( int argn, char** argv )
{
  logtastic::addLogFile( "sculpted_tests.log" );
  logtastic::init( argv[0], "TEST" );

  std::cout << "\nTesting Sculpting Capabilities!\n";

  if ( argn >= 2 )
  {
    INFO_LOG( "Initialising Cylindrical Boolean Map" );
    S3D::cylindricalBool_map* map = new S3D::cylindricalBool_map( threeVector( 0.0 ), argv[1], 1, 2, 3 );

    S3D::sculptedCylinder cyl( threeVector( 0.0 ), S3D::unit_threeVector_z, map->radius(), map->length(), map );
    // S3D::cylinder cyl( threeVector( 0.0 ), S3D::unit_threeVector_z, map->radius(), map->length() );

    std::cout << "Created Sculpted Cylinder.\nProperties:\n\n";
    std::cout << " Center : " << cyl.center() << '\n';
    std::cout << " Radius : " << cyl.radius() << '\n';
    std::cout << " Length : " << cyl.height() << '\n';
    std::cout << " Step Length (Rad) : " << map->stepRad() << '\n';
    std::cout << " Step Length (Len) : " << map->stepLen() << '\n';

    INFO_LOG( "Basic Properties Printed" );
    INFO_LOG( "Testing Other 3D Methods" );
    std::cout << "\nTesting Crossing Points\n";

    S3D::ray ray1( makeThreeVector( 0.00, 0.0, 0.0 ), makeThreeVector( 3.0, 0.0, 0.0 ) );
    S3D::ray ray2( makeThreeVector( 0.0, 0.0, -2.0 ), makeThreeVector( 0.0, 0.0, 2.0 ) );
    S3D::ray ray3( threeVector( -0.1 ), makeThreeVector( 0.0, 0.5, 0.0 ) );
    S3D::ray ray4( makeThreeVector( 0.9, 0.0, 0.0 ), makeThreeVector( 3.0, 0.0, 0.0 ) );

    std::cout << "Crossing Ray 1 : " << cyl.crosses( ray1 ) << " Intersection : " << cyl.intersect( ray1 ) << '\n';
    std::cout << "Crossing Ray 2 : " << cyl.crosses( ray2 ) << " Intersection : " << cyl.intersect( ray2 ) << '\n';
    std::cout << "Crossing Ray 3 : " << cyl.crosses( ray3 ) << '\n';
    std::cout << "Crossing Ray 4 : " << cyl.crosses( ray3 ) << '\n';


    INFO_LOG( "End of Tests" );
  }
  else
  {
    std::cout << "Specify Map File!\n";
    ERROR_LOG( "Map File Not Specified!" );
  }


  std::cout << std::endl;
  logtastic::stop();
  return 0;
}

