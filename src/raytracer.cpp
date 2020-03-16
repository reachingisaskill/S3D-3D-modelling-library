
#include "S3D_raytracer.h"

#include "S3D_line.h"
#include "S3D_manager.h"

namespace S3D
{

  rayTracer::rayTracer()
  {
  }

  rayTracer::~rayTracer()
  {
  }

  beam rayTracer::traceRay( threeVector start, threeVector dir )
  {
    line the_beam( start, dir );

    Object3DMapT& objectMap = manager::getInstance()->_objects;
    Pointer3DContainerT::iterator obj_it = objectMap[0].begin();

    double current_distance = 1.0e20;
    object_3D_base* current_object = nullptr;

    std::cout << start << ", " << dir << " :\n";
    while( obj_it != objectMap[0].end() )
    {
      std::cout << "  " << (*obj_it);
      if ( (*obj_it)->crosses( &the_beam ) )
      {
        threeVector intersect = (*obj_it)->intersect( &the_beam );
        double test = (intersect - start).mod();
          
        if ( test < current_distance )
        {
          current_distance = test;
          current_object = (*obj_it);
          std::cout << "(" << test << " : " << intersect << ")";
        }
      }

      std::cout << "\n";
      ++obj_it;
    }


    beam b;

    if ( current_object != nullptr )
    {
      colour c = current_object->getColour();
      b.red = c.getRed();
      b.green = c.getGreen();
      b.blue = c.getBlue();
    }

    std::cout << std::endl;
    return b;
  }

}

