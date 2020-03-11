
#include "S3D_printer.h"

#include <iomanip>
#include <fstream>

namespace S3D
{

////////////////////////////////////////////////////////////////////////////////
// print_base Methods
////////////////////////////////////////////////////////////////////////////////

  unsigned long int print_base::shapeCounter = 0;


  print_base::print_base( const char* shape ) :
    _def( std::string( "SHAPE_") + stdexts::stringConvert( shapeCounter ) ),
    _shape( shape ),
    _params(),
    _colourRed( S3D_DEFAULT_COLOUR_RED ),
    _colourGreen( S3D_DEFAULT_COLOUR_GREEN ),
    _colourBlue( S3D_DEFAULT_COLOUR_BLUE ),
    _opacity( S3D_DEFAULT_COLOUR_OPAC )
  {
    ++shapeCounter;
  }

  print_base::print_base( const char* DEF, const char* shape ) :
    _def( DEF ),
    _shape( shape ),
    _params(),
    _colourRed( S3D_DEFAULT_COLOUR_RED ),
    _colourGreen( S3D_DEFAULT_COLOUR_GREEN ),
    _colourBlue( S3D_DEFAULT_COLOUR_BLUE ),
    _opacity( S3D_DEFAULT_COLOUR_OPAC )
  {
    ++shapeCounter;
  }

  print_base::print_base( const print_base& s ) :
    _def( s._def ),
    _shape( s._shape ),
    _params( s._params ),
    _colourRed( s._colourRed ),
    _colourGreen( s._colourGreen ),
    _colourBlue( s._colourBlue ),
    _opacity( s._opacity )
  {
    ++shapeCounter;
  }

  print_base& print_base::operator= ( const print_base& s )
  {
    _def =  s._def;
    _shape =  s._shape;
    _params =  s._params;
    _colourRed =  s._colourRed;
    _colourGreen =  s._colourGreen;
    _colourBlue =  s._colourBlue;
    _opacity = s._opacity;

    return *this;
  }

  print_base::~print_base()
  {
  }

  void print_base::addParam( std::string key, std::string val )
  {
    _params.insert( std::make_pair( key, val ) );
  }

  void print_base::setColour( double r, double g, double b )
  {
    _colourRed = r;
    _colourGreen = g;
    _colourBlue = b;
  }

  void print_base::setOpacity( double op )
  {
    _opacity = 1.0 - op;
  }


////////////////////////////////////////////////////////////////////////////////
// print_primitives Member Functions
////////////////////////////////////////////////////////////////////////////////



  print_primitive::print_primitive( const char* shape ) :
    print_base( shape ),
    _position( 0.0 ),
    _direction( unit_threeVector_z ),
    _rotAxis( unit_threeVector_z ),
    _rotAngle( 0.0 )
  {
  }

  print_primitive::print_primitive( const char* DEF, const char* shape ) :
    print_base( DEF, shape ),
    _position( 0.0 ),
    _direction( unit_threeVector_z ),
    _rotAxis( unit_threeVector_z ),
    _rotAngle( 0.0 )
  {
  }

  print_primitive::print_primitive( const print_primitive& s ) :
    print_base( s ),
    _position( s._position ),
    _direction( s._direction ),
    _rotAxis( s._rotAxis ),
    _rotAngle( s._rotAngle )
  {
  }

  print_primitive& print_primitive::operator= ( const print_primitive& s )
  {
    print_base::operator= ( s );
    _position =  s._position;
    _direction =  s._direction;
    _rotAxis = s._rotAxis;
    _rotAngle = s._rotAngle;

    return *this;
  }

  print_primitive::~print_primitive()
  {
  }

  void print_primitive::setPosition( threeVector v )
  {
    _position = v;
  }

  void print_primitive::setDirection( threeVector v )
  {
    _direction = v;
    // TODO : Set Rotation parameters using this threeVector!
  }

  void print_primitive::setRotation( double x, double y, double z, double phi )
  {
    _rotAxis = makeThreeVector( x, y, z );
    _rotAngle = phi;
  }





#ifdef __S3D_PRINTER__USE_X3D__

  void print_primitive::print( std::ostream& out, unsigned int indent ) const
  {
    std::string dent( "" );
    for ( unsigned int i = 0; i < indent; ++i ) dent += "  ";

    std::string def = " DEF=\"" + getDef() + '"';
    std::string params( "" );
    for ( std::map< std::string, std::string >::const_iterator it = _params.begin(); it != _params.end(); ++it )
      params += ' ' + it->first + "=\"" + it->second + '"';

    out << dent << "<Transform translation=\"" << _position[0] << ' ' << _position[1] << ' ' << _position[2] << "\">\n";
    out << dent << "  <Shape" << def << ">\n";
    out << dent << "    <" << _shape << params << ">\n";
    out << dent << "      <Appearance>\n";
    out << dent << "        <Material diffuseColor=\"" << _colourRed << ' ' << _colourGreen << ' '<< _colourBlue << "\">\n";
    out << dent << "      </Appearance>\n";
    out << dent << "    </" << _shape << ">\n";
    out << dent << "  </Shape>\n";
    out << dent << "</Transform>\n";
  }

#endif
#ifdef __S3D_PRINTER__USE_VRML__

  void print_primitive::print( std::ostream& out, unsigned int indent ) const
  {
    std::string dent( "" );
    for ( unsigned int i = 0; i < indent; ++i ) dent += "  ";

    std::string def = "# DEF = " + this->getDef() + '\n';
    std::string params( "" );
    for ( std::map< std::string, std::string >::const_iterator it = _params.begin(); it != _params.end(); ++it )
      params += dent + "        " + it->first + "    " + it->second + '\n';

    out << def;
    out << dent << "Transform {\n";
    out << dent << "  rotation       " << std::setprecision( 8 ) << _rotAxis << _rotAngle << '\n';
    out << dent << "  translation    " << std::setprecision( 8 ) << _position[0] << ' ' << _position[1] << ' ' << _position[2] << "\n\n";
    out << dent << "  children [\n";
    out << dent << "    Shape {\n";
    out << dent << "      appearance Appearance {\n";
    out << dent << "        material Material {\n";
    out << dent << "          diffuseColor  " << _colourRed << ' ' << _colourGreen << ' '<< _colourBlue << '\n';
    out << dent << "          transparency  " << _opacity << '\n';
    out << dent << "        }\n";
    out << dent << "      }\n";
    out << dent << "      geometry " << _shape << " {\n" << params;
    out << dent << "      }\n";
    out << dent << "    }\n";
    out << dent << "  ]\n";
    out << dent << "}\n\n";

  }

#endif


////////////////////////////////////////////////////////////////////////////////
// print_lineSet Member Functions
////////////////////////////////////////////////////////////////////////////////


  print_lineSet::print_lineSet() :
    print_base( "IndexedLineSet" )
  {
  }

  print_lineSet::print_lineSet( const char* c ) :
    print_base( c, "IndexedLineSet" )
  {
  }

  print_lineSet::print_lineSet( const print_lineSet& s ) :
    print_base( s ),
    _points( s._points )
  {
  }

  print_lineSet& print_lineSet::operator= ( const print_lineSet& s )
  {
    print_base::operator= ( s );
    _points = s._points;
    return *this;
  }

  print_lineSet::~print_lineSet()
  {
  }

  void print_lineSet::addPoint( threeVector p )
  {
    _points.push_back( p );
  }


#ifdef __S3D_PRINTER__USE_X3D__

  void print_lineSet::print( std::ostream& out, unsigned int indent ) const
  {
    std::string dent( "" );
    for ( unsigned int i = 0; i < indent; ++i ) dent += "  ";

    std::string def = " DEF=\"" + getDef() + '"';
    std::string params( "" );
    for ( std::map< std::string, std::string >::const_iterator it = _params.begin(); it != _params.end(); ++it )
      params += ' ' + it->first + "=\"" + it->second + '"';

    out << dent << "<Shape" << def << ">\n";
    out << dent << "  <" << _shape << params << ">\n";
    out << dent << "    <Appearance>\n";
    out << dent << "      <Material diffuseColor=\"" << _colourRed << ' ' << _colourGreen << ' '<< _colourBlue << "\">\n";
    out << dent << "    </Appearance>\n";
    out << dent << "  </" << _shape << ">\n";
    out << dent << "</Shape>\n";
  }

#endif
#ifdef __S3D_PRINTER__USE_VRML__

  void print_lineSet::print( std::ostream& out, unsigned int indent ) const
  {
    std::string dent( "" );
    for ( unsigned int i = 0; i < indent; ++i ) dent += "  ";

    std::string def = "# DEF = " + this->getDef() + '\n';
    std::string params( "" );
    for ( std::map< std::string, std::string >::const_iterator it = _params.begin(); it != _params.end(); ++it )
      params += dent + "        " + it->first + "    " + it->second + '\n';

    std::stringstream points( "" );
    std::vector< threeVector>::const_iterator it = _points.begin();
    points << "[ " << (*it)[0] << ' ' << (*it)[1] << ' ' << (*it)[2];
    for ( ++it; it != _points.end(); ++it )
      points << ", " << (*it)[0] << ' ' << (*it)[1] << ' ' << (*it)[2];
    points << " ]\n";

    std::stringstream pointNums( "[ " );
    pointNums << "[ " << 0;
    for ( unsigned int i = 1; i < _points.size(); ++i )
      pointNums << ", " << i;
    pointNums << " ]\n";


    out << def;
    out << dent << "Shape {\n";
    out << dent << "  appearance Appearance {\n";
    out << dent << "    material Material {\n";
    out << dent << "      emissiveColor  " << _colourRed << ' ' << _colourGreen << ' '<< _colourBlue << '\n';
    out << dent << "      transparency   " << _opacity << '\n';
    out << dent << "    }\n";
    out << dent << "  }\n";
    out << dent << "  geometry " << _shape << " {\n" << params;
    out << dent << "    coord Coordinate {\n";
    out << dent << "      point " << points.str();
    out << dent << "    }\n";
    out << dent << "    coordIndex " << pointNums.str();
    out << dent << "  }\n";
    out << dent << "}\n";

  }

#endif


////////////////////////////////////////////////////////////////////////////////
// print_faceSet Member Functions
////////////////////////////////////////////////////////////////////////////////


  print_faceSet::print_faceSet() :
    print_base( "IndexedFaceSet" )
  {
  }

  print_faceSet::print_faceSet( const char* c ) :
    print_base( c, "IndexedFaceSet" )
  {
  }

  print_faceSet::print_faceSet( const print_faceSet& s ) :
    print_base( s ),
    _coords( s._coords ),
    _nextCoord( s._nextCoord )
  {
  }

  print_faceSet& print_faceSet::operator= ( const print_faceSet& s )
  {
    print_base::operator= ( s );
    _coords = s._coords;
    _nextCoord = s._nextCoord;
    return *this;
  }

  print_faceSet::~print_faceSet()
  {
  }

  void print_faceSet::addPoint( long int num, threeVector p )
  {
    _coords.push_back( p );
    _nextCoord.push_back( num );
  }


#ifdef __S3D_PRINTER__USE_X3D__

  void print_faceSet::print( std::ostream& out, unsigned int indent ) const
  {
    std::string dent( "" );
    for ( unsigned int i = 0; i < indent; ++i ) dent += "  ";

    std::string def = " DEF=\"" + getDef() + '"';
    std::string params( "" );
    for ( std::map< std::string, std::string >::const_iterator it = _params.begin(); it != _params.end(); ++it )
      params += ' ' + it->first + "=\"" + it->second + '"';

    out << dent << "<Shape" << def << ">\n";
    out << dent << "  <" << _shape << params << ">\n";
    out << dent << "    <Appearance>\n";
    out << dent << "      <Material diffuseColor=\"" << _colourRed << ' ' << _colourGreen << ' '<< _colourBlue << "\">\n";
    out << dent << "    </Appearance>\n";
    out << dent << "  </" << _shape << ">\n";
    out << dent << "</Shape>\n";
    WRONG WRONG WRONG WRONG!!
  }

#endif
#ifdef __S3D_PRINTER__USE_VRML__

  void print_faceSet::print( std::ostream& out, unsigned int indent ) const
  {
    std::string dent( "" );
    for ( unsigned int i = 0; i < indent; ++i ) dent += "  ";

    std::string def = "# DEF = " + this->getDef() + '\n';
    std::string params( "" );
    for ( std::map< std::string, std::string >::const_iterator it = _params.begin(); it != _params.end(); ++it )
      params += dent + "        " + it->first + "    " + it->second + '\n';

    std::stringstream points( "" );
    std::vector< threeVector>::const_iterator it1 = _coords.begin();
    points << "[ " << (*it1)[0] << ' ' << (*it1)[1] << ' ' << (*it1)[2];
    for ( ++it1; it1 != _coords.end(); ++it1 )
      points << ", " << (*it1)[0] << ' ' << (*it1)[1] << ' ' << (*it1)[2];
    points << " ]\n";

    std::stringstream pointNums( "[ " );
    std::vector< long int >::const_iterator it2 = _nextCoord.begin();
    pointNums << "[ " << (*it2);
    for ( ++it2; it2 != _nextCoord.end(); ++it2 )
      pointNums << ", " << (*it2);
    pointNums << " ]\n";


    out << def;
    out << dent << "Shape {\n";
    out << dent << "  appearance Appearance {\n";
    out << dent << "    material Material {\n";
    out << dent << "      emissiveColor  " << _colourRed << ' ' << _colourGreen << ' '<< _colourBlue << '\n';
    out << dent << "      transparency   " << _opacity << '\n';
    out << dent << "    }\n";
    out << dent << "  }\n";
    out << dent << "  geometry " << _shape << " {\n" << params;
    out << dent << "    coord Coordinate {\n";
    out << dent << "      point " << points.str();
    out << dent << "    }\n";
    out << dent << "    coordIndex " << pointNums.str();
    out << dent << "  }\n";
    out << dent << "}\n";

  }

#endif


////////////////////////////////////////////////////////////////////////////////
// Printer Member Functions
////////////////////////////////////////////////////////////////////////////////


  printer::printer( std::string fileName ) :
    _fileName( fileName )
  {
  }

  printer::~printer()
  {
  }

#ifdef __S3D_PRINTER__USE_X3D__

  void printer::write() const
  {
    std::ofstream of( _fileName.c_str() );

    of << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n\n"
       << "<X3D version=\"3.2\">\n\n  <head>\n";

    for ( std::map< std::string, std::string >::const_iterator it = _metaData.begin(); it != _metaData.end(); ++it )
    {
      of << "    <meta name=\"" << it->first << "\" content=\"" << it->second << "\"/>\n";
    }
    of << "\n  </head>\n\n  <Scene>\n\n";

    for ( shapePtrVecT::const_iterator it = _shapes.begin(); it != _shapes.end(); ++it )
    // for ( std::vector< object_base& >::const_iterator it = _objects.begin(); it != _objects.end(); ++it )
    {
      (*it)->print( of, 2 );
    }

    of << "  </Scene>\n\n</X3D>\n";
  }

#endif

#ifdef __S3D_PRINTER__USE_VRML__

  void printer::write() const
  {
    std::ofstream of( _fileName.c_str() );

    of << "#VRML V2.0 utf8\n\n";

    for ( std::map< std::string, std::string >::const_iterator it = _metaData.begin(); it != _metaData.end(); ++it )
    {
      of << "# " << it->first << " : " << it->second << "\n";
    }
    of << '\n';

    of << "Viewpoint { \n  position 10 0 0\n  orientation 0 1 0 1.570796\n}\n\n";
    of << "Background { \n  skyColor [ 0.6 0.6 0.6 ]\n}\n\n";

    for ( shapePtrVecT::const_iterator it = _shapes.begin(); it != _shapes.end(); ++it )
    // for ( std::vector< object_base& >::const_iterator it = _objects.begin(); it != _objects.end(); ++it )
    {
      (*it)->print( of, 1 );
    }

    of << '\n';
  }

#endif

}

