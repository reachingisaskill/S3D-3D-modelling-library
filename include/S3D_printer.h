
#ifndef __S3D_PRINTER_H__
#define __S3D_PRINTER_H__

#include "S3D_realVector.h"
#include "S3D_rotation.h"
#include "S3D_defs.h"

#include "stdexts.h"

#include <iostream>
#include <string>
#include <map>
#include <vector>


// #define __S3D_PRINTER__USE_X3D__
#define __S3D_PRINTER__USE_VRML__

namespace S3D
{

  class object_base;


  class print_base
  {
    friend class printer;

    private:
      static unsigned long int shapeCounter;

      std::string _def;

    protected:
      virtual void print( std::ostream&, unsigned int = 0 ) const = 0;

      std::string _shape;

      std::map< std::string, std::string > _params;
      double _colourRed;
      double _colourGreen;
      double _colourBlue;
      // Cannot be arsed with materials and textures!

    public:
      print_base( const char* );
      print_base( const char*, const char* );
      print_base( const print_base& );
      print_base& operator= ( const print_base& );
      virtual ~print_base();

      const std::string& getDef() const { return _def; }

      void addParam( std::string, std::string );
      void setColour( double, double, double );
  };


  class print_primitive : public print_base
  {
    private :
      threeVector _position;
      threeVector _direction;
      threeVector _rotAxis;
      double _rotAngle;

    protected :
      virtual void print( std::ostream&, unsigned int = 0 ) const;

    public :
      print_primitive( const char* );
      print_primitive( const char*, const char* );
      print_primitive( const print_primitive& );
      print_primitive& operator= ( const print_primitive& );
      virtual ~print_primitive();

      void setPosition( threeVector );
      void setDirection( threeVector );
      void setRotation( double, double, double, double );
  };


  class print_lineSet : public print_base
  {
    private:
      std::vector< threeVector > _points;

    protected :
      virtual void print( std::ostream&, unsigned int = 0 ) const;

    public:
      print_lineSet();
      print_lineSet( const char* );
      print_lineSet( const print_lineSet& );
      print_lineSet& operator= ( const print_lineSet& );
      virtual ~print_lineSet();

      void addPoint( threeVector );
  };


  class print_faceSet : public print_base
  {
    private:
      std::vector< threeVector > _coords;
      std::vector< long int > _nextCoord;

    protected :
      virtual void print( std::ostream&, unsigned int = 0 ) const;

    public:
      print_faceSet();
      print_faceSet( const char* );
      print_faceSet( const print_faceSet& );
      print_faceSet& operator= ( const print_faceSet& );
      virtual ~print_faceSet();

      void addPoint( long int, threeVector );
  };


  class printer : public stdexts::non_copyable
  {
    typedef std::vector< stdexts::sharedPtr< print_base > > shapePtrVecT;

    private :
      std::string _fileName;
      std::map< std::string, std::string > _metaData;
      shapePtrVecT _shapes;
      //std::vector< object_base& > _objects;

    public :
      printer( std::string );
      virtual ~printer();

      void addShape( print_base* s ) { _shapes.push_back( stdexts::sharedPtr< print_base >( s ) ); }
      void addData( std::string n, std::string v ) { _metaData.insert( std::make_pair( n, v ) ); }

      // void print( object_base& o ) { _objects.push_back( o ); }

      void write() const;
  };

}

#endif // __S3D_PRINTER_H__

