//#define LOGTASTIC_DEBUG_OFF

#include "S3D_object_base.h"

#include "S3D_defs.h"
#include "S3D_manager.h"

#include "logtastic.h"

#include <cmath>


namespace S3D
{

  object_base::object_base( material_base* m ) :
    base(),
    _material( m )
  {
  }


  object_base::~object_base()
  {
  }


//  threeVector object_base::_calculateSnellsLawIn( threeVector in, threeVector normal ) const
//  {
//    double refraction_ratio = manager::getInstance()->getWorld()->getMaterial()->getRefractiveIndex() / this->getMaterial()->getRefractiveIndex();
//    threeVector cross = crossProduct( in, normal );
//    double cross_mod = cross.mod(); // = Sin( theta_1 )
//    if ( cross_mod == 0.0 ) return in;
//    
//    double product = refraction_ratio * cross_mod;
//    DEBUG_STREAM << "PRODUCT = " << product;
//
//    double delta_theta = std::asin( refraction_ratio * cross_mod ) - std::asin( cross_mod );
//    threeVector out = rotation( cross, delta_theta ) * in;
//    DEBUG_STREAM << "SnellsLawOut: Delta theta = " << delta_theta << " | Incoming = " << in << " | Out = " << out << " | " << cross_mod << " , " << refraction_ratio;
//    return out;
//  }
//
//
//  threeVector object_base::_calculateSnellsLawOut( threeVector in, threeVector normal ) const
//  {
//    double refraction_ratio = this->getMaterial()->getRefractiveIndex() / manager::getInstance()->getWorld()->getMaterial()->getRefractiveIndex();
//    threeVector cross = crossProduct( in, normal );
//    double cross_mod = cross.mod(); // = Sin( theta_1 )
//    if ( cross_mod == 0.0 ) return in; // Parallel
//
//
//    double delta_theta = std::asin( refraction_ratio * cross_mod ) - std::asin( cross_mod );
//    threeVector out = rotation( cross, delta_theta ) * in;
//    DEBUG_STREAM << "SnellsLawOut: Delta theta = " << delta_theta << " | Incoming = " << in << " | Out = " << out << " | " << cross_mod << " , " << refraction_ratio;
//    return out;
//  }
//
//
//  threeVector object_base::_calculateReflection( threeVector in, threeVector normal ) const
//  {
//    return in - 2.0 * normal * in * normal;
//  }
}

