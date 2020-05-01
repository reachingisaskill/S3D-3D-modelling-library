
#ifndef _SIMULATION_3D_DEFINITIONS_H_
#define _SIMULATION_3D_DEFINITIONS_H_

#include "S3D_vector.h"
#include "S3D_spectrum.h"


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
#define S3D_DEFAULT_COLOUR S3D::spectrum( S3D_DEFAULT_COLOUR_RED, S3D_DEFAULT_COLOUR_GREEN, S3D_DEFAULT_COLOUR_BLUE )
#endif


namespace S3D
{

  const double PI = 3.1415926535897932384626434;

  const double E = 2.7182818284590452353602875;

  const threeVector unit_threeVector_x = makeThreeVector( 1.0, 0.0, 0.0 );
  const threeVector unit_threeVector_y = makeThreeVector( 0.0, 1.0, 0.0 );
  const threeVector unit_threeVector_z = makeThreeVector( 0.0, 0.0, 1.0 );

  const threeVector defaultDirection = unit_threeVector_z;

  const threeVector the_origin = makeThreeVector( 0.0, 0.0, 0.0 );

  const spectrum colour_red = spectrum( 1.0, 0.0, 0.0 );
  const spectrum colour_green = spectrum( 0.0, 1.0, 0.0 );
  const spectrum colour_blue = spectrum( 0.0, 0.0, 1.0 );
  const spectrum colour_black = spectrum( 0.0, 0.0, 0.0 );
  const spectrum colour_white = spectrum( 1.0, 1.0, 1.0 );
  const spectrum colour_lightGrey = spectrum( 0.75, 0.75, 0.75 );
  const spectrum colour_grey = spectrum( 0.5, 0.5, 0.5 );
  const spectrum colour_darkGrey = spectrum( 0.25, 0.25, 0.25 );

  const double epsilon = 1.0e-9;
}


#endif // _SIMULATION_3D_DEFINITIONS_H_

