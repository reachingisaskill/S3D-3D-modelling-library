
#ifndef __S3D__ROTATION_H__
#define __S3D__ROTATION_H__

#include "S3D_vector.h"

#include "S3D_defs.h"


namespace S3D
{

  class rotation
  {
    private:
      threeVector _axis;
      double _theta;

      double _rotMax[3][3];

    protected:
      explicit rotation( double );

      void calcMatrix();
      void unCalcMatrix();

    public:
      rotation();
      // Specify axis of rotation and angle (rotations applied at the origin)
      rotation( threeVector, double );
      virtual ~rotation();

      virtual const double& getAngle() const { return _theta; }
      virtual void setAngle( double an ) { _theta = an; this->calcMatrix(); }

      virtual const threeVector& getAxis() const { return _axis; }
      virtual void setAxis( threeVector v ) { _axis = v; this->calcMatrix(); }

      threeVector rotateVector( threeVector ) const;
      threeVector rotateVectorBackwards( threeVector ) const;
      virtual threeVector operator()( threeVector vec ) const { return this->rotateVector( vec ); }


      virtual bool operator==( const rotation& ) const;
      virtual bool operator!=( const rotation& r ) const { return ( ! operator==( r ) ); }
      // rotation operator-();

      virtual rotation& operator*=( rotation );

      friend rotation operator-( rotation );
      friend threeVector operator*( rotation, threeVector );
      friend rotation operator*( rotation, rotation );
      friend threeVector operator/( rotation, threeVector );
  };

  double degreesToRadians( double );
  double radiansToDegrees( double );

  double angle( threeVector, threeVector );
  threeVector cross( threeVector, threeVector );


  rotation operator-( rotation );
  threeVector operator*( rotation, threeVector );
  rotation operator*( rotation, rotation );
  threeVector operator/( rotation, threeVector );

}

#endif // __S3D__ROTATION_H__

