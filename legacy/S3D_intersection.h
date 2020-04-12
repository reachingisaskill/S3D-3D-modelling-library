
#ifndef __S3D__INTERSECTION_HPP__
#define __S3D__INTERSECTION_HPP__


#include "S3D_allShapes.h"


namespace S3D
{

  class intersection_base : public volume_base
  {
    private:
      volume_base* _shape1;
      volume_base* _shape2;

    protected:
      const volume_base* getShape1() const { return _shape1; }
      const volume_base* getShape2() const { return _shape2; }

    public:
      intersection_base( volume_base*, volume_base*, double, threeVector, rotation );
      virtual ~intersection_base();

      // virtual bool contains( const threeVector* pt ) const;
      // virtual bool Contains( const threeVector* pt ) const;
      // virtual bool crosses( const line* ) const;
      // virtual bool crosses( const ray* ) const;
      // virtual double distance( const line* ) const;
      // virtual stdexts::fifo< double > distances( const line* ) const;
      // virtual threeVector intersect( const line* ) const;
      
      virtual void rotate( rotation );
      virtual void rotateAbout( rotation, threeVector );
  };

}

#endif // __S3D__INTERSECTION_HPP__

