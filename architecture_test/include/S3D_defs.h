
#ifndef __SIMULATION_3D__DEFINITIONS_H__
#define __SIMULATION_3D__DEFINITIONS_H__

#include "S3D_vector.h"
#include "S3D_beam.h"


#ifndef S3D_DEFAULT_COLOUR_RED
#define S3D_DEFAULT_COLOUR_RED 1.0 
#endif

#ifndef S3D_DEFAULT_COLOUR_GREEN
#define S3D_DEFAULT_COLOUR_GREEN 1.0
#endif

#ifndef S3D_DEFAULT_COLOUR_BLUE
#define S3D_DEFAULT_COLOUR_BLUE 1.0
#endif

#ifndef S3D_DEfAULT_COLOUR
#define S3D_DEFAULT_COLOUR S3D::colour( S3D_DEFAULT_COLOUR_RED, S3D_DEFAULT_COLOUR_GREEN, S3D_DEFAULT_COLOUR_BLUE )
#endif


namespace S3D
{

  const double PI = 3.1415926535897932384626434;

  const threeVector unit_threeVector_x = makeThreeVector( 1.0, 0.0, 0.0 );
  const threeVector unit_threeVector_y = makeThreeVector( 0.0, 1.0, 0.0 );
  const threeVector unit_threeVector_z = makeThreeVector( 0.0, 0.0, 1.0 );

  const threeVector defaultDirection = unit_threeVector_z;

  const threeVector the_origin = makeThreeVector( 0.0, 0.0, 0.0 );

  const colour colour_red = colour( 1.0, 0.0, 0.0 );
  const colour colour_green = colour( 0.0, 1.0, 0.0 );
  const colour colour_blue = colour( 0.0, 0.0, 1.0 );
  const colour colour_black = colour( 0.0, 0.0, 0.0 );
  const colour colour_white = colour( 1.0, 1.0, 1.0 );
  const colour colour_lightGrey = colour( 0.75, 0.75, 0.75 );
  const colour colour_grey = colour( 0.5, 0.5, 0.5 );
  const colour colour_darkGrey = colour( 0.25, 0.25, 0.25 );

  const double epsilon = 1.0e-9;
}


#endif // __SIMULATION_3D__DEFINITIONS_H__

