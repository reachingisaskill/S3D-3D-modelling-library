
#ifndef __S3D__GLOBAL_H__
#define __S3D__GLOBAL_H__

#include "S3D_colour.h"
#include "S3D_point.h"
#include "S3D_rotation.h"
#include "S3D_base.h"

#include "stdexts.h"


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
  const colour colour_lightGrey = colour( 0.25, 0.25, 0.25 );
  const colour colour_grey = colour( 0.5, 0.5, 0.5 );
  const colour colour_darkGrey = colour( 0.75, 0.75, 0.75 );

  const colour colour_invisible = colour( 0.0, 0.0, 0.0, 0.0 );

}


#endif // __S3D__GLOBAL_H__

