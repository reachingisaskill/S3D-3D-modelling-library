
#ifndef __S3D__STEP_H__
#define __S3D__STEP_H__


#include "S3D_realVector.h"


namespace S3D
{

  class step
  {
    private:
      threeVector _start;
      threeVector _direction;

    public:
      step( threeVector, threeVector );
      step( const step& );
      step& operator=( const step& );
      ~step();

      threeVector getPosition() const;
      void setPosition( threeVector vec );

      threeVector getCenter() const { return this->getPosition(); }
      void setCenter( threeVector vec ) { this->setPosition( vec ); }

      const threeVector& getStart() const { return _start; }
      void setStart( threeVector vec ) { _start = vec; }

      threeVector getEnd() const;
      void setEnd( threeVector vec );

      const threeVector& getDirection() const { return _direction; }
      void setDirection( threeVector vec ) { _direction = vec; }

      bool operator==( const step& ) const;
      bool operator!=( const step& stp ) const { return ( ! this->operator==( stp ) ); }

  };

}

#endif // __S3D__STEP_H__

