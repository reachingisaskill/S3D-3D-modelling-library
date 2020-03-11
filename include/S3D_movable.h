
#ifndef __S3D__MOVABLE_H__
#define __S3D__MOVABLE_H__

/*
 * This class will contain the necessary functionality to implement
 *  3D motion. Stepping Algorithms: RK4, Euler, etc (if I can be arsed)
 *
 * To be inherited by any object that wishes to move.
 *
 * Similar classes may be created to alter local dimensions of simple 
 *  objects as a function of time (Extra Features)
 *
 * Just add a force and tell it to step! (Not quite sure on units yet...)
 *
 * Boundary crossing & intersecting methodologies to be deterrmined.
 *
 * Should this class hold local copies of position and direction or use
 * ones found in the inheritor class?
 *
 * Perhaps CRTP so that it can get a this pointer and cast to useful 
 * type?
 *
 */

namespace S3D
{

  class movable 
  {
  };

}

#endif // __S3D__MOVABLE_H__

