
#ifndef __S3D__BASE_H__
#define __S3D__BASE_H__


#include "S3D_defs.h"

#include "S3D_colour.h"
#include "S3D_rotation.h"

#include "stdexts.h"


namespace S3D
{
  class object_base;
  class control;
  class point;
  class line;
  class ray;


  class object_base : public stdexts::non_copyable
  {
    friend class manager;

    private:
      bool _isOwned;
      colour _colour;
      int _layer;
      threeVector _position;
      rotation _rotation;

    protected:

    public:
      object_base( threeVector = threeVector( 0.0 ), rotation = rotation() );
      virtual ~object_base();

      colour& getColour() { return _colour; }
      const colour& getColour() const { return _colour; }
      void setColour( colour c ) { _colour = c; }

      const int& getLayer() const { return _layer; }
      // void setLayer( int );

      virtual void setCenter( threeVector pos ) { _position = pos; }
      virtual const threeVector& getCenter() const { return _position; }

      // virtual void setRotation( rotation rot ) { _rotation = rot; } // USE ROTATE FUNCTION!
      virtual const rotation& getRotation() const { return _rotation; }

      virtual void rotate( rotation );
      virtual void rotateAbout( rotation, threeVector );

  };


  class object_3D_base : public object_base
  {
    private:

    protected:

    public:
      object_3D_base( threeVector = threeVector( 0.0 ), rotation = rotation() );

      virtual ~object_3D_base();

      // Pure virtual functions to handle intersection, containment and collision.
      virtual bool contains( threeVector vec ) const { return this->contains( &vec ); }
      virtual bool Contains( threeVector vec ) const { return this->Contains( &vec ); }
      virtual bool contains( const threeVector* ) const = 0;
      virtual bool Contains( const threeVector* ) const = 0;
      virtual bool crosses( const line* ) const = 0;
      virtual bool crosses( const ray* ) const = 0;
      virtual double distance( const line* ) const = 0;
      virtual threeVector intersect( const line* ) const = 0;

      // Volume only
//      virtual stdexts::fifo< double > distances( const line* ) const = 0;
  };

}

#endif // __S3D__BASE_H__

