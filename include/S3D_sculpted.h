
#ifndef __S3D__SCULPTED_H__
#define __S3D__SCULPTED_H__

#include "S3D_allShapes.h"
#include "S3D_mapping.h"

#include "stdexts.h"


namespace S3D
{

  class cylindricalBool_map : public boolFieldMap< boolPixel< 2 > >
  {
    private:
      unsigned long int _numZ;
      unsigned long int _numX;
      double _rangeZ;
      double _rangeX;
      double _minZ;
      double _maxZ;
      double _minX;
      double _maxX;

      stdexts::dll< boolPixel< 2 > > _load( std::string, unsigned int, unsigned int, unsigned int ) const;
      long unsigned int _pixelNum( double, double ) const;

    public:
      cylindricalBool_map( threeVector, std::string, unsigned int, unsigned int, unsigned int );

      virtual double radius() const { return _rangeX; }
      virtual double length() const { return _rangeZ; }

      virtual double stepLen() const { return _rangeZ / _numZ; }
      virtual double stepRad() const { return _rangeX / _numX; }

      virtual bool value( threeVector v ) const { return this->live( v ); }
      virtual bool live( threeVector ) const;
  };



//  The Idea : 
  // Make a cylinder and chop bits off.
  // Calculate interactions mostly using contained/Contained.
  // All hit points projected onto outer cylinder object.
  //
  // An approximation - but should be close enough!
  //
  class sculptedCylinder : public cylinder 
  {
    private:
      stdexts::sharedPtr< cylindricalBool_map > _fieldMap;

    public:
      sculptedCylinder( threeVector, rotation, double, double, cylindricalBool_map* );
      sculptedCylinder( cylindricalBool_map* );

      virtual bool contains( threeVector pt ) const { return _fieldMap->live( pt ); }
      virtual bool Contains( threeVector pt ) const { return _fieldMap->live( pt ); }

  // print_base classes don't exist - left over from another library
//      virtual print_base* _print() const;
  };
  
}

#endif // __S3D__SCULPTED_H__

