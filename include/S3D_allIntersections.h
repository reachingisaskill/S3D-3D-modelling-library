
#ifndef __S3D__ALL_INTERSECTIONS_H__
#define __S3D__ALL_INTERSECTIONS_H__


#include "S3D_global.h"

#include "S3D_intersection.h"


namespace S3D
{


  class addition : public intersection_base
  {
    private:
    public:
      addition( volume_base*, volume_base*, rotation = rotation() );
      ~addition();

      virtual bool contains( const threeVector* pt ) const;
      virtual bool Contains( const threeVector* pt ) const;
      virtual bool crosses( const line* ) const;
      virtual bool crosses( const ray* ) const;
      virtual double distance( const line* ) const;
      virtual stdexts::fifo< double > distances( const line* ) const;
      virtual threeVector intersect( const line* ) const;
  };

  class subtraction : public intersection_base
  {
    private:
    public:
      subtraction( volume_base*, volume_base*, rotation = rotation() );
      ~subtraction();

      virtual bool contains( const threeVector* pt ) const;
      virtual bool Contains( const threeVector* pt ) const;
      virtual bool crosses( const line* ) const;
      virtual bool crosses( const ray* ) const;
      virtual double distance( const line* ) const;
      virtual stdexts::fifo< double > distances( const line* ) const;
      virtual threeVector intersect( const line* ) const;
  };
 
  class intersection : public intersection_base
  {
    private:
    public:
      intersection( volume_base*, volume_base*, rotation = rotation() );
      ~intersection();
      
      virtual bool contains( const threeVector* pt ) const;
      virtual bool Contains( const threeVector* pt ) const;
      virtual bool crosses( const line* ) const;
      virtual bool crosses( const ray* ) const;
      virtual double distance( const line* ) const;
      virtual stdexts::fifo< double > distances( const line* ) const;
      virtual threeVector intersect( const line* ) const;
  };
 
  class exclusion : public intersection_base
  {
    private:
    public:
      exclusion( volume_base*, volume_base*, rotation = rotation() );
      ~exclusion();
      
      virtual bool contains( const threeVector* pt ) const;
      virtual bool Contains( const threeVector* pt ) const;
      virtual bool crosses( const line* ) const;
      virtual bool crosses( const ray* ) const;
      virtual double distance( const line* ) const;
      virtual stdexts::fifo< double > distances( const line* ) const;
      virtual threeVector intersect( const line* ) const;
  };


//     // Thats the dream....
//  addition* operator+( volume_base*, volume_base* );
//  subtraction* operator-( volume_base*, volume_base* );
//  intersection* operator*( volume_base*, volume_base* );
//  exclusion* operator^( volume_base*, volume_base* );
 
}

#endif // __S3D__ALL_INTERSECTIONS_H__

