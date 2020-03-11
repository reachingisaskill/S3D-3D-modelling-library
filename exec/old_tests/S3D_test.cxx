
#include <iostream>

#include "S3D_allShapes.h"
#include "S3D_allSurfaces.h"
#include "S3D_intersections.h"

using namespace S3D;


int main ( int, char** )
{
  std::cout << "Testing Line Crossing" << std::endl;

  
  std::cout << "Creating 2 Planes" << std::endl;
  plane* p1 = new plane( threeVector( 0.0 ), makeThreeVector( 0.0, 0.0, 1.0 ) );
  surface* p2 = new square_plane( threeVector( 0.0 ), makeThreeVector( 0.0, 0.0, 1.0 ), 1.0, 1.0 );

  std::cout << "\n\tPLANE 1 : Infinite Plane\n" << std::endl;

  std::cout << "Creating 6 Points" << std::endl;
  threeVector pt1( 0.0 );
  threeVector pt2( 0.0 );
  pt2[2] = 1.0;
  threeVector pt3( 0.0 );
  pt3[2] = -10.0;
  threeVector pt4( 0.0 );
  pt4[0] = 1.4;
  pt4[1] = -2.6;
  pt4[2] = -24.0;
  threeVector pt5( 0.0 );
  pt5[0] = 1.0;
  pt5[1] = 1.1;
  pt5[2] = 1.0;
  threeVector pt6 = makeThreeVector( 1.0, 1.1, -1.0 );
  std::cout << pt1 << "\n" << pt2 << "\n" << pt3 << "\n" << pt4 << "\n" << pt5 << "\n" << pt6 << "\n";

  std::cout << "\nDistances : \n" << p1->distance( pt1 ) << ", " << p1->distance( pt2 ) << ", " << p1->distance( pt3 ) << ", " << p1->distance( pt4 ) << ", " << p1->distance( pt5 ) << ", " << p1->distance( pt6 ) << std::endl;
  
  std::cout << "\nCreating 4 Lines" << std::endl;

  ray l1( pt2, pt4 );
  ray l2( pt2, pt5 );
  ray l3( pt2, pt3 );
  ray l4( pt5, pt6 );

  std::cout << "Lines : \n";
  std::cout << l1.start() << l1.end() << l1.direction() << "\n";
  std::cout << l2.start() << l2.end() << l2.direction() << "\n";
  std::cout << l3.start() << l3.end() << l3.direction() << "\n";
  std::cout << l4.start() << l4.end() << l4.direction() << "\n";

  std::cout << "Distances :\n";
  std::cout << "1 - 1 : " << l1.separation(l1) << l1.distance(l1) << "\n";
  std::cout << "1 - 2 : " << l1.separation(l2) << l1.distance(l2) << "\n";
  std::cout << "1 - 3 : " << l1.separation(l3) << l1.distance(l3) << "\n";
  std::cout << "1 - 4 : " << l1.separation(l4) << l1.distance(l4) << "\n";
  std::cout << "2 - 1 : " << l2.separation(l1) << l2.distance(l1) << "\n";
  std::cout << "2 - 3 : " << l2.separation(l3) << l2.distance(l3) << "\n";
  std::cout << "2 - 4 : " << l2.separation(l4) << l2.distance(l4) << "\n";
  std::cout << "3 - 4 : " << l3.separation(l4) << l3.distance(l4) << "\n";

  std::cout << "Crossing : \n" << p1->crosses( l1 ) << ", " << p1->crosses( l2 ) << ", " << p1->crosses( l3 ) << ", " << p1->crosses( l4 ) << std::endl;

  std::cout << "Intersections : \n" << p1->intersect( l1 ) << "\n" << p1->intersect( l2 ) << "\n" << p1->intersect( l3 ) << "\n" << p1->intersect( l4 ) << std::endl;





  std::cout << "\n\tPLANE 2 : Finite Square Plane\n" << std::endl;

  std::cout << "Crossing : \n" << p2->crosses( l1 ) << ", " << p2->crosses( l2 ) << ", " << p2->crosses( l3 ) << ", " << p2->crosses( l4 ) << std::endl;

  std::cout << "Intersections : \n" << p2->intersect( l1 ) << "\n" << p2->intersect( l2 ) << "\n" << p2->intersect( l3 ) << "\n" << p2->intersect( l4 ) << std::endl;




  std::cout << "\n\nCreating 1 Box" << std::endl;

  simple_shape *v1 = new box( 1.9, 2.0, 6.0, threeVector( 0.0 ) );

  std::cout << "Box Planes:\n";
  for ( unsigned int i = 0; i < v1->numSurfaces(); ++i )
  {
    std::cout << "  Plane "<< i << " : " << v1->surfaces()[i]->center() << "\n";
    std::cout << "    Width = " << ((square_plane*)v1->surfaces()[i])->width() << ", Length = " << ((square_plane*)v1->surfaces()[i])->length() << "\n";
  }

  std::cout << "\nContaining : \n" << v1->contains( pt1 ) << ", " << v1->contains( pt2 ) << ", " << v1->contains( pt3 ) << ", " << v1->contains( pt4 ) << ", " << v1->contains( pt5 ) << ", " << v1->contains( pt6 ) << std::endl;

  std::cout << "Crossing : \n" << v1->crosses( l1 ) << ", " << v1->crosses( l2 ) << ", " << v1->crosses( l3 ) << ", " << v1->crosses( l4 ) << std::endl;






  std::cout << "\n\nCreating Cylindrical Surface" << std::endl;

  surface* s1 = new cylinder_wall_section( makeThreeVector( 0.0, 0.0, 1.0 ), unit_threeVector_y, 2.0, 3.0 );
  surface* s2 = new cylinder_wall( makeThreeVector( 0.0, 0.0, 1.0 ), unit_threeVector_y, 2.0 );

  std::cout << "\nCreating 2 New Lines" << std::endl;

  ray l5( makeThreeVector( 0.0, 2.6, 2.0 ), makeThreeVector( 0.0, 2.6, -2.0 ) );
  ray l6( makeThreeVector( -2.0, -2.0, -2.0 ), makeThreeVector( 1.0, 0.0, 1.0 ), 3.0 );

  std::cout << l5.start() << l5.end() << l5.direction() << "\n";
  std::cout << l6.start() << l6.end() << l6.direction() << "\n";


  std::cout << "\n\n\tSURFACE 1 : Cylinder Wall Section" << std::endl;


  std::cout << "\nDistances : \n" << s1->distance( pt1 ) << ", " << s1->distance( pt2 ) << ", " << s1->distance( pt3 ) << ", " << s1->distance( pt4 ) << ", " << s1->distance( pt5 ) << ", " << s1->distance( pt6 ) << std::endl;

  std::cout << "Crossing : \n" << s1->crosses( l1 ) << ", " << s1->crosses( l2 ) << ", " << s1->crosses( l3 ) << ", " << s1->crosses( l4 ) << ", " << s1->crosses( l5 ) << ", " << s1->crosses( l6 ) << std::endl;

  std::cout << "Intersections : \n" << s1->intersect( l1 ) << "\n" << s1->intersect( l2 ) << "\n" << s1->intersect( l3 ) << "\n" << s1->intersect( l4 ) << "\n" << s1->intersect( l5 ) << "\n" << s1->intersect( l6 ) << std::endl;


  std::cout << "\n\n\tSURFACE 2 : Cylinder Wall" << std::endl;


  std::cout << "\nDistances : \n" << s2->distance( pt1 ) << ", " << s2->distance( pt2 ) << ", " << s2->distance( pt3 ) << ", " << s2->distance( pt4 ) << ", " << s2->distance( pt5 ) << ", " << s2->distance( pt6 ) << std::endl;

  std::cout << "Crossing : \n" << s2->crosses( l1 ) << ", " << s2->crosses( l2 ) << ", " << s2->crosses( l3 ) << ", " << s2->crosses( l4 ) << ", " << s2->crosses( l5 ) << ", " << s2->crosses( l6 ) << std::endl;

  std::cout << "Intersections : \n" << s2->intersect( l1 ) << "\n" << s2->intersect( l2 ) << "\n" << s2->intersect( l3 ) << "\n" << s2->intersect( l4 ) << "\n" << s2->intersect( l5 ) << "\n" << s2->intersect( l6 ) << std::endl;









  std::cout << "\n\nPoints :\n";
  std::cout << pt1 << "\n" << pt2 << "\n" << pt3 << "\n" << pt4 << "\n" << pt5 << "\n" << pt6 << "\n";

  std::cout << "\nLines : \n";
  std::cout << l1.start() << l1.end() << l1.direction() << "\n";
  std::cout << l2.start() << l2.end() << l2.direction() << "\n";
  std::cout << l3.start() << l3.end() << l3.direction() << "\n";
  std::cout << l4.start() << l4.end() << l4.direction() << "\n";
  std::cout << l5.start() << l5.end() << l5.direction() << "\n";
  std::cout << l6.start() << l6.end() << l6.direction() << "\n";

  std::cout << "\n\nCreating CAVITY!" << std::endl;

  volume_base* v2 = new cylinder( makeThreeVector( 0.0, -5.0, -5.0 ), unit_threeVector_z, 5.0, 12.0 );
  std::cout << "Center : " << v2->center() << "Direction : " << unit_threeVector_z <<  ", Radius : " << 5.0 << ", Length " << 11.0 << std::endl;

  std::cout << "\nContaining : \n" << v2->contains( pt1 ) << ", " << v2->contains( pt2 ) << ", " << v2->contains( pt3 ) << ", " << v2->contains( pt4 ) << ", " << v2->contains( pt5 ) << ", " << v2->contains( pt6 ) << std::endl;

  std::cout << "Crossing : \n" << v2->crosses( l1 ) << ", " << v2->crosses( l2 ) << ", " << v2->crosses( l3 ) << ", " << v2->crosses( l4 ) << ", " << v2->crosses( l5 ) << ", " << v2->crosses( l6 ) << std::endl;

  std::cout << "A little test: " << v2->contains( makeThreeVector( 0.0, -0.001, 1.0 ) );


  delete p1;
  delete p2;
  delete v1;
  delete v2;
  delete s1;
  delete s2;



///////////////////////////////////////////////////////////////////////////////////////////////////////

  std::cout << "\n\nTesting with realistic cavity.\n" << std::endl;

  simple_shape* v3 = new cylinder( threeVector( 0.0 ), unit_threeVector_z, 0.5, 0.4 );

  std::cout << "Center : " << v3->center() << "Direction : " << unit_threeVector_z <<  ", Radius : " << 0.5 << ", Length " << 0.4 << std::endl;

  std::cout << "SURFACE[0] : " << v3->surfaces()[0]->center() << std::endl;
  std::cout << "SURFACE[1] : " << v3->surfaces()[1]->center() << std::endl;
  std::cout << "SURFACE[2] : " << v3->surfaces()[2]->center() << std::endl;

  ray l7( makeThreeVector( 0.0, 0.0, 0.18 ), makeThreeVector( 0.0, 0.0, 0.21) );
  ray l8( makeThreeVector( 0.45, 0.0, 0.1 ), makeThreeVector( 0.51, 0.0, 0.1 ) );
  ray l9( makeThreeVector( 0.33, 0.33, -0.1 ), makeThreeVector( 0.4, 0.4, -0.1 ) );
  ray l10( makeThreeVector( -0.1, 0.1, -0.1 ), makeThreeVector( 0.1, -0.1, -0.15) );
  ray l11( makeThreeVector( 0.0, 0.0, -0.18 ), makeThreeVector( 0.0, 0.0, -0.21) );

  std::cout << "Crossing : \n" << v3->crosses( l7 ) << ", " << v3->crosses( l8 ) << ", " << v3->crosses( l9 ) << ", " << v3->crosses( l10 ) << ", " << v3->crosses( l11 ) << std::endl;

  std::cout << "\nIntersections:\n" << v3->intersect( l7 ) << "\n" << v3->intersect( l8 ) << "\n" << v3->intersect( l9 ) << "\n" << v3->intersect( l10 ) << "\n" << v3->intersect( l11 ) << "\n";

  std::cout << "\nCrossing SURFACE[1] : " << v3->surfaces()[1]->crosses( l7 ) << ", " << v3->surfaces()[1]->crosses( l8 ) << ", " << v3->surfaces()[1]->crosses( l9 ) << ", " << v3->surfaces()[1]->crosses( l10 ) << ", " << v3->surfaces()[1]->crosses( l11 ) << std::endl;

  delete v3;

///////////////////////////////////////////////////////////////////////////////////////////////////////

  std::cout << "\n\nTesting with MORE realistic cavity.\n" << std::endl;

  volume_base* v4 = new subtraction( new subtraction( new cylinder( threeVector( 0.0 ), unit_threeVector_z, 0.5, 0.4 ), new cylinder( threeVector( 0.0 ), unit_threeVector_z, 0.494, 0.394 ) ), new cylinder( threeVector( 0.0 ), unit_threeVector_z, 0.0, 0.403 ) );
  // volume_base* v4 = new subtraction( new cylinder( threeVector( 0.0 ), unit_threeVector_z, 0.5, 0.4 ), new cylinder( threeVector( 0.0 ), unit_threeVector_z, 0.496, 0.396 ) );
  subtraction* sub = (subtraction*)v4;

  std::cout << "Center : " << v4->center() << "Direction : " << unit_threeVector_z <<  ", Radius : " << 0.5 << ", Length " << 0.4 << std::endl;

  std::cout << "Crossing : \n" << v4->crosses( l7 ) << ", " << v4->crosses( l8 ) << ", " << v4->crosses( l9 ) << ", " << v4->crosses( l10 ) << ", " << v4->crosses( l11 ) << std::endl;

  std::cout << "\nIntersections:\n" << v4->intersect( l7 ) << "\n" << v4->intersect( l8 ) << "\n" << v4->intersect( l9 ) << "\n" << v4->intersect( l10 ) << "\n" << v4->intersect( l11 ) << "\n";

  ray l12( makeThreeVector( 0.4, 0.4, 0.1 ), makeThreeVector( -0.4, 0.4, 0.1 ) );
  ray l13( makeThreeVector( -0.03, -0.06, 0.201 ), makeThreeVector( -0.03, -0.06, 0.21 ) );

  std::cout << "Another Test : " << v4->crosses( l12 ) << ", " << v4->crosses( l13 ) << "\n";
  std::cout << "Another Test : " << v4->intersect( l12 ) << ", " << v4->intersect( l13 ) << "\n";

  std::cout << "Checking Surfaces : " << sub->vol1()->crosses( l13 ) << " " << ((subtraction*)sub->vol1())->vol1()->crosses( l13 ) << " " << ((subtraction*)sub->vol1())->vol2()->crosses( l13 ) << " " << sub->vol2()->crosses( l13 ) << "\n";


  ray l14( makeThreeVector( -0.36, -0.36, -0.02 ), makeThreeVector( -0.34, -0.34, +0.02 ) );

  std::cout << "\n\nYET Another Test : " << v4->crosses( l14 ) << " : " << v4->intersect( l14 ) << "\n";


  delete v4;


///////////////////////////////////////////////////////////////////////////////////////////////////////

/*
  
  std::cout << "\n\nCylinder Intersection Checking\n" << std::endl;

  cylinder* c1 = new cylinder( threeVector( 0.0 ), unit_threeVector_z, 0.5, 0.4 );

  std::cout << "Center : " << c1->center() << ", Direction : " << unit_threeVector_z <<  ", Radius : " << 0.5 << ", Length " << 0.4 << std::endl;

  std::cout << "Creating a Couple of Rays\n\n";

  ray r1( makeThreeVector( -0.501, 0.001, -0.03 ), makeThreeVector( -0.499, 0.001, 0.03 ) );
  ray r2( makeThreeVector( -0.499, 0.0, -0.03 ), makeThreeVector( -0.501, 0.0, 0.03 ) );
  ray r3( makeThreeVector( 0.0, 0.0, 0.201 ), makeThreeVector( 0.0, 0.0, 0.21 ) );

  std::cout << "Crossing : \n" << c1->crosses( r1 ) << ", " << c1->crosses( r2 ) << ", " << c1->crosses( r3 ) << std::endl;

  std::cout << "\nCrossing Surfaces: " << c1->surfaces()[0]->crosses( r1 ) << " " << c1->surfaces()[1]->crosses( r1 ) << " " << c1->surfaces()[2]->crosses( r1 );
  std::cout << "\nCrossing Surfaces: " << c1->surfaces()[0]->crosses( r2 ) << " " << c1->surfaces()[1]->crosses( r2 ) << " " << c1->surfaces()[2]->crosses( r2 );
  std::cout << "\nCrossing Surfaces: " << c1->surfaces()[0]->crosses( r3 ) << " " << c1->surfaces()[1]->crosses( r3 ) << " " << c1->surfaces()[2]->crosses( r3 );
  std::cout << "\n\n";

  std::cout << "\nIntersections:\n" << c1->intersect( r1 ) << "\n" << c1->intersect( r2 ) << "\n" << c1->intersect( r3 ) << "\n";


  delete c1;

*/



  std::cout << std::endl;
  return 0;
}




/*

int main( int, char** )
{
  std::cout << "Hello!" << std::endl;

  std::cout << "\n3 BOXES...\n" << std::endl;

  threeVector pt1;
  pt1[0] = 1;
  pt1[1] = 2;
  pt1[2] = 3;

  box b1;
  box b2( 1, 1, 1);
  box b3( 2, 2, 2, pt1 );


  threeVector pt;
  pt[0] = 1;
  pt[1] = 2;
  pt[2] = 3;

  std::cout << "\nConsider a point : ( 1, 2, 3 )" << std::endl;
  std::cout << "\nBox 1 " << ( b1.contains( pt ) ? "contains" : "does not contain" ) << " the point \'p\'";
  std::cout << "\nBox 2 " << ( b2.contains( pt ) ? "contains" : "does not contain" ) << " the point \'p\'";
  std::cout << "\nBox 3 " << ( b3.contains( pt ) ? "contains" : "does not contain" ) << " the point \'p\'";
  std::cout << "\n" << std::endl;



  std::cout << "3 CYLINDERS...\n" << std::endl;

  vertical_cylinder c1;
  vertical_cylinder c2( 1, 1);
  vertical_cylinder c3( 2, 2, pt1 );


  std::cout << "\nConsider a point : ( 1, 2, 3 )" << std::endl;
  std::cout << "\nBox 1 " << ( c1.contains( pt ) ? "contains" : "does not contain" ) << " the point \'p\'";
  std::cout << "\nBox 2 " << ( c2.contains( pt ) ? "contains" : "does not contain" ) << " the point \'p\'";
  std::cout << "\nBox 3 " << ( c3.contains( pt ) ? "contains" : "does not contain" ) << " the point \'p\'";
  std::cout << "\n" << std::endl;




  std::cout << "1 SUBTRACTION...\n" << std::endl;

  vertical_cylinder c4( 1, 1, pt1 );

  subtraction< vertical_cylinder, vertical_cylinder > S = c3 - c4;


  std::cout << "\nCylinder Intersection 1 " << ( S.contains( pt ) ? "contains" : "does not contain" ) << " the point \'p\'";

  pt[0] = 2.5;
  pt[1] = 2.0;
  pt[2] = 3.0;

  std::cout << "\nCylinder Intersection 1 " << ( S.contains( pt ) ? "contains" : "does not contain" ) << " the point ( 2.5, 2.0, 3.0 )";
  std::cout << "\n" << std::endl;



  std::cout << "ALL TOGETHER NOW... \n" << std::endl;

  volume* volList[10];
  volList[0] = &b1;
  volList[1] = &b2;
  volList[2] = &b3;
  volList[3] = &c1;
  volList[4] = &c2;
  volList[5] = &c3;
  volList[6] = &c4;
  volList[7] = &S;

  for ( int i = 0; i < 7; ++i )
  {
    std::cout << "\nVolume " << i << " " << ( volList[i]->contains( pt1 ) ? "contains" : "does not contain" ) << " the point ( 1, 2, 3 )";
  }

  std::cout << "\n" << std::endl;


  std::cout << "SOME CRAZY SHAPES!\n" << std::endl;
  subtraction< addition< box, box >, box > S1 = ( (b1 + b2) - b3 );
  intersection< exclusion< addition< vertical_cylinder, box >, vertical_cylinder >, box> S2 = ( ( ( c1 + b2) ^ c2) * b3 );


  volList[8] = &S1;
  volList[9] = &S2; 

  for ( int i = 7; i < 10; ++i )
  {
    std::cout << "\nVolume " << i << " " << ( volList[i]->contains( pt1 ) ? "contains" : "does not contain" ) << " the point ( 1, 2, 3 )";
  }

  std::cout << "\n" << std::endl;


  return 0;
}
*/

