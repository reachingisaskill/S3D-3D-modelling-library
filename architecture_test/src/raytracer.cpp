
#include "S3D_raytracer.h"

#include "S3D_line.h"
#include "S3D_manager.h"
#include "S3D_light_base.h"

namespace S3D
{

  rayTracer::rayTracer() :
    _layer( 0 )
  {
  }

  rayTracer::~rayTracer()
  {
  }

  beam rayTracer::traceRay( threeVector start, threeVector dir ) const
  {
    return this->_traceRay( beam(), start, dir );
  }

  beam rayTracer::_traceRay( beam b, threeVector start, threeVector dir ) const
  {
    line the_beam( start, dir );

    Object3DMapT& objectMap = manager::getInstance()->_objects;
    Pointer3DContainerT::iterator obj_it = objectMap[_layer].begin();
    Pointer3DContainerT::iterator obj_end = objectMap[_layer].end();

    double current_distance = 1.0e20;
    object_3D_base* current_object = nullptr;
    threeVector current_intersect;

    std::cout << start << ", " << dir << " :\n";
    while( obj_it != obj_end )
    {
      if ( (*obj_it)->crosses( &the_beam ) )
      {
        threeVector intersect = (*obj_it)->intersect( &the_beam );
        double test = (intersect - start).mod();
          
        if ( test < current_distance )
        {
          current_distance = test;
          current_object = (*obj_it);
          current_intersect = intersect;
          std::cout << "(" << test << " : " << intersect << ")";
        }
      }

      std::cout << "\n";
      ++obj_it;
    }


    if ( current_object == nullptr )
      return beam();
    

    LightingMapT& lights = manager::getInstance()->_lights;
    LightingContainerT::iterator light_it = lights[_layer].begin();
    LightingContainerT::iterator light_end = lights[_layer].end();

    beam light_in;

    while( light_it != light_end )
    {
      light_in += (*light_it)->calculateFlux( current_intersect, this );
      ++light_it;
    }

    b = current_object->getMaterial()->interact( light_in, 0.0, this );

//    if ( current_object != nullptr )
//    {
//      colour c = current_object->getColour();
//      b.red += c.getRed();
//      b.green += c.getGreen();
//      b.blue += c.getBlue();
//    }

//    std::cout << std::endl;
    return b;
  }


  bool rayTracer::isVisible( threeVector start, threeVector end ) const
  {
    threeVector separation = end - start;
    line the_beam( start, separation );
    double distance = separation.mod();

    Object3DMapT& objectMap = manager::getInstance()->_objects;
    Pointer3DContainerT::iterator obj_it = objectMap[_layer].begin();

    std::cout << "     Vis " << start << " -> " << end << std::endl;
    while( obj_it != objectMap[_layer].end() )
    {
      if ( (*obj_it)->crosses( &the_beam ) )
      {
        threeVector intersect = (*obj_it)->intersect( &the_beam );
        double test = (intersect - start).mod();
        std::cout << test;
          
        if ( test < distance )
        {
          std::cout << "FAIL" << std::endl;
          return false;
        }
      }
      ++obj_it;
    }
    std::cout << "SUCCEED" << std::endl;

    return true;
  }

}

