
#ifndef __S3D__ALL_LINES_H__
#define __S3D__ALL_LINES_H__


#include "S3D_line.h"

#include <vector>


namespace S3D
{

  class ray : public line
  {
    private:
      threeVector _end;
      double _length;

      threeVector _calcGrad( const threeVector&, const threeVector& );

    protected:
      // virtual print_base* _print() const;
      
    public:
      ray( threeVector, threeVector );
      ray( threeVector, rotation, double );
      ray( threeVector, threeVector, double );
      virtual ~ray() {}

      virtual const threeVector& getEnd() const { return _end; }

      virtual const double& getLength() const { return _length; }
      virtual void setLength( double );
  };

  
  class spline : public line
  {
    private:
      struct splinePoint
      {
        threeVector position;
        threeVector gradient;
      };
      std::vector< splinePoint > _points;

    protected:
      // virtual print_base* _print() const;

    public:
      spline( threeVector, threeVector );

      double getLength() const;

      void addPoint( threeVector, threeVector );
  };

}

#endif // __S3D__ALL_LINES_H__

