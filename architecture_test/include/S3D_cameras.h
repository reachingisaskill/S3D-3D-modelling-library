
#ifndef __S3D__CAMERAS_H__
#define __S3D__CAMERAS_H__

#include "S3D_camera_base.h"

#include "S3D_convergence.h"
#include "S3D_random.h"


namespace S3D
{
////////////////////////////////////////////////////////////////////////////////////////////////////
  // Line Scan Camera

  class camera_lineScan : public camera_base
  {
    private :
      double _width; // Physical sizes
      double _height;

    protected:

    public:
      // Position, Rotation, size_x, size_y
      camera_lineScan( tracer_base*, double, double );

      // Use the implicit copy, and move constructors
//      camera_linescan( const camera_linescan& );
//      camera_linescan& operator=( const camera_linescan& );

      // Make sure its virtual
      virtual ~camera_lineScan();

      // Override pure virtual function
      virtual void shutter();
  };


////////////////////////////////////////////////////////////////////////////////////////////////////
  // Perfect Pin Hole Camera

  class camera_pinhole : public camera_base
  {
    private :

    protected:

    public:
      // Position, Rotation, FieldofView
      camera_pinhole( tracer_base*, double );

      // Make sure its virtual
      virtual ~camera_pinhole();

      // Override pure virtual function
      virtual void shutter();
  };


////////////////////////////////////////////////////////////////////////////////////////////////////
  // Pin Hole Camera with random sampling

  class camera_sampledPinhole : public camera_base
  {
    private :
      unsigned int _sampleRate;

    protected:
      void runSample( unsigned int, unsigned int ) const;

    public:
      // raytracer, FieldofView, samples per pixel
      camera_sampledPinhole( tracer_base*, double, unsigned int sampleRate = 1 );

      // Set Samples per pixel
      void setSampleRate( unsigned int i ) { _sampleRate = i; }

      // Make sure its virtual
      virtual ~camera_sampledPinhole();

      // Override pure virtual function
      virtual void shutter();
  };


////////////////////////////////////////////////////////////////////////////////////////////////////
  // Perspective Camera with random sampling

  class camera_perspective : public camera_base
  {
    private :
      double _length; // How far back the focal point is (means the camera "lens" is shorter!)
      double _width;
      double _height;
      unsigned int _sampleRate;

    protected:
      void runSample( unsigned int, unsigned int ) const;

    public:
      // raytracer, FieldofView, width, height
      camera_perspective( tracer_base*, double, double, double );

      // Set Samples per pixel
      void setSampleRate( unsigned int i ) { _sampleRate = i; }

      // Make sure its virtual
      virtual ~camera_perspective();

      // Override pure virtual function
      virtual void shutter();
  };

////////////////////////////////////////////////////////////////////////////////////////////////////
  // Perspective Camera with random sampling & convergence algorithm

  class camera_convergePerspective : public camera_base
  {
    private :
      double _length; // How far back the focal point is (means the camera "lens" is shorter!)
      double _width;
      double _height;
      convergence_error _converge;
      random::halton _hX;
      random::halton _hY;

    protected:
      void runSample( unsigned int, unsigned int ) const;

    public:
      // raytracer, FieldofView, width, height, convergence criteria
      camera_convergePerspective( tracer_base*, double, double, double, double c = 0.1 );

      // Make sure its virtual
      virtual ~camera_convergePerspective();

      // Hard limit just in case
      void setMaxSamples( unsigned int i ) { _converge.setMaxSamples( i ); }

      // Override pure virtual function
      virtual void shutter();
  };

}

#endif // __S3D__CAMERAS_H__

