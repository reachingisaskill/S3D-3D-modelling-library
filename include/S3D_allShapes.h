
#ifndef __ALL_SHAPES_HPP__
#define __ALL_SHAPES_HPP__


#include "S3D_volume.h"


namespace S3D
{

  class sphere : public simple_shape
  {
    private:
      double _radius;

      void makeSurfaces() {}

    public:
      sphere();
      sphere( double, threeVector );
      virtual ~sphere();

      virtual const double& getRadius() const { return _radius; }
      virtual void setRadius( double r ) { _radius = r; }

      virtual bool contains( const threeVector* ) const;
      virtual bool Contains( const threeVector* ) const;
      virtual bool crosses( const line* l ) const;
      virtual bool crosses( const ray* l ) const;
      virtual double distance( const line* ) const;
      virtual stdexts::fifo< double > distances( const line* ) const;
      virtual interaction intersect( const line* ) const;
  };

  class box : public simple_shape
  {
    private:
      double _halfWidth_x;
      double _halfWidth_y;
      double _halfWidth_z;

      double _width_x;
      double _width_y;
      double _width_z;

      void makeSurfaces();

    protected:
      // virtual print_base* _print() const;

    public:

      box();
      box( double, double, double, threeVector = threeVector( 0.0 ), rotation = rotation() );
      virtual ~box();

      virtual const double& getWidthX() const { return _width_x; }
      virtual const double& getWidthY() const { return _width_y; }
      virtual const double& getWidthZ() const { return _width_z; }

//      virtual void setWidthX( double );
//      virtual void setWidthY( double );
//      virtual void setWidthZ( double );

      virtual bool contains( const threeVector* ) const;
      virtual bool Contains( const threeVector* ) const;
      virtual bool crosses( const line* ) const;
      virtual bool crosses( const ray* ) const;
  };


  class cylinder : public simple_shape
  {
    private:
      line* _centerLine;
      double _radius;
      double _height;
      double _halfHeight;

      void makeSurfaces();

    protected:
    public:
      cylinder();
      cylinder( double, double, threeVector = threeVector( 0.0 ), rotation = rotation() );
      cylinder( const cylinder& );
      virtual ~cylinder();

      virtual double& getRadius() { return _radius; }
      virtual const double& getRadius() const { return _radius; }
      virtual void setRadius( double rad ) { _radius = rad; }

      virtual double& getHeight() { return _height; }
      virtual const double& getHeight() const { return _height; }
      virtual void setHeight( double h ) { _height = h; }

      virtual const line* getCenterLine() const { return _centerLine; }

      virtual bool contains( const threeVector* ) const;
      virtual bool Contains( const threeVector* ) const;
      virtual bool crosses( const line* ) const;
      virtual bool crosses( const ray* ) const;

      virtual void rotate( rotation );
      virtual void rotateAbout( rotation, threeVector );

  };

}

#endif // __ALL_SHAPES_HPP__
