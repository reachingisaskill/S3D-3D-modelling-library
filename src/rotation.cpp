
#include "S3D_rotation.h"

#include "S3D_global.h"

#include <cmath>


namespace S3D
{

  rotation::rotation( double val ) :
    _axis( threeVector( val ) ),
    _theta( val )
  {
    for ( unsigned int i = 0; i < 3; ++i )
      for ( unsigned int j = 0; j < 3; ++j )
        _rotMax[i][j] = val;
  }

  rotation::rotation() :
    _axis( makeThreeVector( 0.0, 0.0, 1.0 ) ),
    _theta( 0.0 )
  {
    calcMatrix();
  }

  rotation::rotation( threeVector ax, double ang ) :
    _axis( ax.norm() ),
    _theta( ang )
  {
    calcMatrix();
  }

  rotation::~rotation()
  {
  }

  void rotation::calcMatrix()
  {
    double cos = std::cos( _theta );
    double sin = std::sin( _theta );

    _rotMax[0][0] = cos + _axis[0]*_axis[0]*(1.0-cos);
    _rotMax[0][1] = _axis[0]*_axis[1]*(1.0-cos) - _axis[2]*sin;
    _rotMax[0][2] = _axis[0]*_axis[2]*(1.0-cos) + _axis[1]*sin;
    _rotMax[1][0] = _axis[1]*_axis[0]*(1.0-cos) + _axis[2]*sin;
    _rotMax[1][1] = cos + _axis[1]*_axis[1]*(1.0-cos);
    _rotMax[1][2] = _axis[1]*_axis[2]*(1.0-cos) - _axis[0]*sin;
    _rotMax[2][0] = _axis[2]*_axis[0]*(1.0-cos) - _axis[1]*sin;
    _rotMax[2][1] = _axis[2]*_axis[1]*(1.0-cos) + _axis[0]*sin;
    _rotMax[2][2] = cos + _axis[2]*_axis[2]*(1.0-cos);
  }

  void rotation::unCalcMatrix()
  {
    double A = _rotMax[0][0] + _rotMax[1][1] + _rotMax[2][2];
    this->_theta = std::acos( ( A - 1.0 ) / 2.0 );

    if ( _theta == 0.0 ) _axis = unit_threeVector_z;
    if ( _theta == S3D::PI ) _axis = unit_threeVector_x;

    double B = _rotMax[1][0] - _rotMax[0][1];
    double C = _rotMax[0][2] - _rotMax[2][0];
    double D = _rotMax[2][1] - _rotMax[1][2];

    this->_axis[0] = 0.5 * D / std::sin( this->_theta );
    this->_axis[1] = 0.5 * C / std::sin( this->_theta );
    this->_axis[2] = 0.5 * B / std::sin( this->_theta );
  }

  threeVector rotation::rotateVector( threeVector vec )
  {
    threeVector newVec( 0.0 );

    for ( unsigned int i = 0; i < 3; ++i )
    {
      for ( unsigned int j = 0; j < 3; ++j )
      {
        newVec[i] += vec[j] * _rotMax[i][j];
      }
    }

    return newVec;
  }

  threeVector rotation::rotateVectorBackwards( threeVector vec )
  {
    threeVector newVec( 0.0 );

    for ( unsigned int i = 0; i < 3; ++i )
    {
      for ( unsigned int j = 0; j < 3; ++j )
      {
        newVec[i] += vec[j] * _rotMax[j][i];
      }
    }

    return newVec;
  }

//  rotation rotation::operator-()
//  {
//    this->_theta = -this->_theta;
//
//    return *this;
//  }

  bool rotation::operator==( const rotation& rot ) const
  {
    if ( rot._theta != this->_theta ) return false;
    if ( rot._axis != this->_axis ) return false;
    return true;
  }

  rotation& rotation::operator*=( rotation rot )
  {
    double tempMax[3][3];
    for ( unsigned int n = 0; n < 3; ++n )
      for ( unsigned int m = 0; m < 3; ++m )
      {
        tempMax[n][m] = _rotMax[n][m];
        _rotMax[n][m] = 0.0;
      }

    for ( unsigned i = 0; i < 3; ++i )
      for ( unsigned j = 0; j < 3; ++j )
        for ( unsigned k = 0; k < 3; ++k )
          this->_rotMax[i][j] += tempMax[k][j] * rot._rotMax[i][k];

    this->unCalcMatrix();

    return *this;
  }

////////////////////////////////////////////////////////////////////////////////

  rotation operator-( rotation r )
  {
    r.setAngle( -r._theta );

    return r;
  }

  threeVector operator*( rotation rot, threeVector vec )
  {
    return rot.rotateVector( vec );
  }

  rotation operator*( rotation rot1, rotation rot2 )
  {
    rotation newRot( 0.0 );

    for ( unsigned i = 0; i < 3; ++i )
      for ( unsigned j = 0; j < 3; ++j )
        for ( unsigned k = 0; k < 3; ++k )
          newRot._rotMax[i][j] += rot1._rotMax[i][k] * rot2._rotMax[k][j];

    newRot.unCalcMatrix();

    return newRot;
  }


  threeVector operator/( rotation rot, threeVector vec )
  {
    return rot.rotateVectorBackwards( vec );
  }

////////////////////////////////////////////////////////////////////////////////

  double degreesToRadians( double deg )
  {
    return ( deg / 180.0 ) * PI;
  }

  double radiansToDegrees( double rad )
  {
    return ( rad / PI ) * 180.0;
  }

  double angle( threeVector v1, threeVector v2 )
  {
    double product = v1 * v2;
    return std::acos( product / ( v1.mod() * v2.mod() ) );
  }

}

