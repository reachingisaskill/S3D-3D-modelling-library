//#define __DEBUG_OFF__

#include "S3D_interaction.h"

#include "S3D_defs.h"

#include "logtastic.h"


namespace S3D
{

  interaction::interaction() :
    _thePoint(),
    _theLine( nullptr ),
    _theObject( nullptr ),
    _distance( 1.0e300 ),
    _distanceSq( 1.0e300 ),
    _surfaceNormal(),
    _refIndexRatio()
  {
  }


  interaction::interaction( point p, const line* l, const object_base* s, threeVector norm, double ref ) :
    _thePoint( p ),
    _theLine( l ),
    _theObject( s ),
    _distance( 0.0 ),
    _distanceSq( 0.0 ),
    _surfaceNormal( norm ),
    _refIndexRatio( ref )
  {
    _distance = ( p - l->getStart()).mod();
    _distanceSq = ( p - l->getStart()).square();
  }


  threeVector interaction::getTransmission() const
  {
    DEBUG_STREAM << "Calculating transmission vector: " << _theLine->getDirection() << " -- " << _surfaceNormal;

    double cos = -_theLine->getDirection() * _surfaceNormal;

    if ( (1.0 - cos) < epsilon )
    {
      DEBUG_LOG( "Incoming vector perpendicular." );
      return _theLine->getDirection();
    }


    double cos_theta_out = 1.0 - _refIndexRatio * _refIndexRatio * cos * cos;

    if ( cos_theta_out < 0.0 )
    {
      DEBUG_LOG( "Total internal reflection case." );
      return this->getReflection(); // Total internal reflection
    }

    threeVector out = _refIndexRatio * _theLine->getDirection() + ( _refIndexRatio * cos - std::sqrt( cos_theta_out ) ) * _surfaceNormal;

    DEBUG_STREAM << "Cos theta_1 = " << cos << ", Cos theta_2 = " << -out * _surfaceNormal << ", In dot Out = " << _theLine->getDirection() * out;
    return out;



//    threeVector cross = crossProduct( _theLine->getDirection(), _surfaceNormal );
//    double cos = -_theLine->getDirection() * _surfaceNormal;
//
//    if ( cross_mod == 0.0 )
//    {
//      DEBUG_LOG( "Incoming vector perpendicular." );
//      return _theLine->getDirection();
//    }
//
//    
//    if ( ( _refIndexRatio * cross_mod ) > 1.0 )
//    {
//      DEBUG_LOG( "Total internal reflection case." );
//      return this->getReflection(); // Total internal reflection
//    }
//
//    double cos = -_theLine->getDirection() * _surfaceNormal;
//    double cos_theta_out = 1.0 - _refIndexRatio * _refIndexRatio * cos * cos;
//
////    threeVector out = _refIndexRatio * _theLine->getDirection() + ( _refIndexRatio * cos - std::sqrt( 1 - _refIndexRatio*_refIndexRatio * cross_mod*cross_mod ) ) * _surfaceNormal;
//    threeVector out = _refIndexRatio * _theLine->getDirection() + ( _refIndexRatio * cos - std::sqrt( 1 - _refIndexRatio*_refIndexRatio * cross_mod*cross_mod ) ) * _surfaceNormal;
//
//    DEBUG_STREAM << "Cos theta_1 = " << cos << ", Cos theta_2 = " << -out * _surfaceNormal << ", In dot Out = " << _theLine->getDirection() * out;
//    return out;
  }


  threeVector interaction::getReflection() const
  {
    return _theLine->getDirection() - 2.0 * _surfaceNormal * _theLine->getDirection() * _surfaceNormal;
  }

}

