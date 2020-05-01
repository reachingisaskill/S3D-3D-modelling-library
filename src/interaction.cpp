#define LOGTASTIC_DEBUG_OFF

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
    _surfacemap(),
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
    _surfacemap(),
    _surfaceNormal( norm ),
    _refIndexRatio( ref )
  {
    _distance = ( p - l->getStart()).mod();
    _distanceSq = ( p - l->getStart()).square();
  }


  threeVector interaction::getTransmission() const
  {
    DEBUG_STREAM << "interaction::getTransmission() : " << _theLine->getDirection() << " -- " << _surfaceNormal;

    double cos = -_theLine->getDirection() * _surfaceNormal;

    if ( (1.0 - cos) < epsilon )
    {
      DEBUG_LOG( "  Incoming vector perpendicular." );
      return _theLine->getDirection();
    }

    double cos_theta_out = 1.0 - _refIndexRatio * _refIndexRatio * ( 1.0 - cos * cos );

    if ( cos_theta_out < 0.0 )
    {
      DEBUG_LOG( "  Total internal reflection case." );
      return this->getReflection(); // Total internal reflection
    }

    threeVector out = _refIndexRatio * _theLine->getDirection() + ( _refIndexRatio * cos - std::sqrt( cos_theta_out ) ) * _surfaceNormal;

    DEBUG_STREAM << "  Cos theta_1 = " << cos << ", Cos theta_2 = " << -out * _surfaceNormal << ", In dot Out = " << _theLine->getDirection() * out;
    return out;
  }


  threeVector interaction::getReflection() const
  {
    threeVector result = _theLine->getDirection() - 2.0 * _surfaceNormal * _theLine->getDirection() * _surfaceNormal;
    DEBUG_STREAM << "Calculating reflected ray. Normal = " << _surfaceNormal << ". Vector = " << _theLine->getDirection() << ". Result = " << result;
    return result;
  }

}

