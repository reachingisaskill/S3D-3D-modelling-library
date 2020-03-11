
#ifndef __S3D__LINE_H__
#define __S3D__LINE_H__


#include "S3D_global.h"
#include "S3D_defs.h"

#include "S3D_allPoints.h"


namespace S3D
{

  class line : public object_base
  {
    private:
      threeVector _direction; // Cached

    protected:
      // virtual print_base* _print() const;

      rotation _getRotation( threeVector v );

    public:
      line( threeVector, threeVector );
      line( threeVector, rotation );
      virtual ~line();

      virtual inline const threeVector& getStart() const { return this->getCenter(); }
      virtual inline void setStart( threeVector p ) { this->setCenter( p ); }

      virtual const threeVector& getDirection() const { return _direction; }

      virtual threeVector separation( const line* ) const;
      virtual double distance( const line* ) const;

      virtual threeVector separation( const threeVector* ) const;
      virtual double distance( const threeVector* ) const;

      virtual void rotate( rotation );
      virtual void rotateAbout( rotation, threeVector );
  };

}

#endif // __S3D__LINE_H__

