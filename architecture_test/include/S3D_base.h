
#ifndef __S3D__BASE_H__
#define __S3D__BASE_H__

#include "S3D_vector.h"
#include "S3D_rotation.h"
#include "S3D_point.h"
#include "S3D_line.h"

#include "stdexts.h"


namespace S3D
{

  class base : public stdexts::non_copyable
  {
    friend class manager;

    private:
      bool _isOwned;
      int _layer;
      point _position;
      rotation _rotation;

    protected:
      object_base( point = point(), rotation = rotation() );

    public:
      virtual ~object_base();

      const int& getLayer() const { return _layer; }

      virtual void setPosition( point pos ) { _position = pos; }
      virtual const point& getPosition() const { return _position; }

      virtual const rotation& getRotation() const { return _rotation; }
      virtual void setRotation( rotation r ) { _rotation = r; }

      virtual void rotate( rotation );
      virtual void rotateAbout( rotation, threeVector );

  };
}

#endif // __S3D__BASE_H__

