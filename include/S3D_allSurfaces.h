
#ifndef __SIMULATION_3D__SURFACES_H__
#define __SIMULATION_3D__SURFACES_H__


#include "S3D_surface.h"
#include "S3D_line.h"


namespace S3D
{

  /////////////////////////////////////////////////////////////////////////////////


  class plane : public surface
  {
    private:
    public:
      plane( threeVector, rotation );

      virtual double area() const { return 0.0; }
      virtual double distance( const threeVector* ) const;
      virtual double distance( const line* ) const;
      virtual bool crosses( const ray* ) const;
      virtual bool crosses( const line* ) const;
      virtual threeVector intersect( const line* ) const;
  };


  /////////////////////////////////////////////////////////////////////////////////


  class square_plane : public plane
  {
    private:
      double _width;
      double _length;
      double _half_width;
      double _half_length;

    public:
      square_plane( threeVector, rotation, double, double );

      virtual const double& getWidth() const { return _width; }
      virtual const double& getLength() const { return _length; }
//      virtual void setWidth( double );
//      virtual void setLength( double );

      virtual double area() const;
      virtual bool crosses( const ray* ) const;
      virtual bool crosses( const line* ) const;
  };


  /////////////////////////////////////////////////////////////////////////////////


  class circular_plane : public plane
  {
    private:
      double _radius;

    public:
      circular_plane( threeVector, rotation, double );

      virtual const double& getRadius() const { return _radius; }
//      virtual void setRadius( double );

      virtual double area() const;
      virtual bool crosses( const ray* ) const;
  };


  /////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////


  class cylinder_wall : public surface
  {
    private:
      double _radius;
      line _centerLine;

    protected:
      const line& getCenterLine() const { return _centerLine; }

    public:
      cylinder_wall( threeVector, rotation, double );

      virtual const double& getRadius() const { return _radius; }
//      virtual void setRadius( double );

      virtual double area() const { return 0.0; }

      virtual double distance( const threeVector* ) const;
      virtual double distance( const line* ) const;
      virtual bool crosses( const ray* ) const;
      virtual bool crosses( const line* ) const;
      virtual threeVector intersect( const line* ) const;

      virtual void rotate( rotation );
      virtual void rotateAbout( rotation, threeVector );
  };


  /////////////////////////////////////////////////////////////////////////////////


  class cylinder_wall_section : public cylinder_wall
  {
    private:
      double _length;
      double _half_length;

    public:
      cylinder_wall_section();
      cylinder_wall_section( threeVector, rotation, double, double );

      virtual const double& getLength() const { return _length; }
//      virtual void setLength( double );

      virtual double area() const { return S3D::PI * 2.0 * getRadius() * _length; }
      virtual bool crosses( const ray* ) const;
  };


  /////////////////////////////////////////////////////////////////////////////////


  // Shaped Surfaces
  /////////////////////////////////////////////////////////////////////////////////
 
  /*
   
  class cutout : public surface
  {
    private:
      unsigned int _numPoints;
      twoVector* _points;
      double _extent;

    public:
      cutout( threeVector, rotation, unsigned int, twoVector* );
      ~cutout() {}

      virtual double area() const;
      virtual double distance( const threeVector* ) const;
      virtual double distance( const line* ) const;
      virtual bool crosses( const ray* ) const;
      virtual bool crosses( const line* ) const;
      virtual threeVector intersect( const line* ) const;

      virtual void rotate( rotation );
      virtual void rotateAbout( rotation, threeVector );
  };

  */

}

#endif // __SIMULATION_3D__SURFACES_H__

