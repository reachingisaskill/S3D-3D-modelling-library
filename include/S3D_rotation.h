
#ifndef S3D_ROTATION_H_
#define S3D_ROTATION_H_

#include "S3D_vector.h"


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

  double vectorAngle( threeVector, threeVector );
  threeVector crossProduct( threeVector, threeVector );
  threeVector findPerpendicular( threeVector );


  rotation operator-( rotation );
  threeVector operator*( rotation, threeVector );
  rotation operator*( rotation, rotation );
  threeVector operator/( rotation, threeVector );

}

#endif // S3D_ROTATION_H_

