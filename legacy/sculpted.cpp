
#ifndef MAX_LINE_SIZE
#define MAX_LINE_SIZE 250
#endif

#include "S3D_sculpted.h"

#include "logtastic.h"

#include <cmath>
#include <cstdlib>

namespace S3D
{

////////////////////////////////////////////////////////////////////////////////
// Cylindrical Boolean Map
////////////////////////////////////////////////////////////////////////////////

  cylindricalBool_map::cylindricalBool_map( threeVector c, std::string filename, unsigned int colZ, unsigned int colX, unsigned int colLive ) :
    S3D::boolFieldMap< boolPixel< 2 > >( c, filename ),
    _numZ( 1 ),
    _numX( 1 ),
    _rangeZ( 0.0 ),
    _rangeX( 0.0 ),
#ifdef INFINITY
    _minZ( INFINITY ),
    _maxZ( -INFINITY ),
    _minX( INFINITY ),
    _maxX( -INFINITY )
#else
    _minZ( 1.0e+300 ),
    _maxZ( -1.0e+300 ),
    _minX( 1.0e+300 ),
    _maxX( -1.0e+300 )
#endif
  {
    stdexts::dll< boolPixel< 2 > > pixels = this->_load( filename, colZ, colX, colLive );
    
    if ( ! pixels ) 
    {
      FAILURE_LOG( "No Pixels found in Field Map" );
      abort();
    }

    stdexts::dll< boolPixel< 2 > >::iterator* it = pixels.first();

    double lastZ = (*it)->position()[ 0 ];
    double lastX = (*it)->position()[ 1 ];
    while ( it )
    {
      if ( (*it)->position()[ 0 ] < _minZ ) _minZ = (*it)->position()[ 0 ];
      if ( (*it)->position()[ 1 ] < _minX ) _minX = (*it)->position()[ 1 ];
      if ( (*it)->position()[ 0 ] > _maxZ ) _maxZ = (*it)->position()[ 0 ];
      if ( (*it)->position()[ 1 ] > _maxX ) _maxX = (*it)->position()[ 1 ];

      if ( (*it)->position()[ 0 ] > lastZ )
      {
        ++_numZ;
        lastZ = (*it)->position()[ 0 ];
      }
      if ( (*it)->position()[ 1 ] > lastX )
      {
        ++_numX;
        lastX = (*it)->position()[ 1 ];
      }

      it = it->next();
    }

    _rangeZ = _maxZ - _minZ;
    _rangeX = _maxX - _minX;

    boolPixel< 2 >* map = new boolPixel< 2 >[ _numX * _numZ ];

  //  std::cout << "Max Z : " << _maxZ << '\n';
  //  std::cout << "Max X : " << _maxX << '\n';
  //  std::cout << "Min Z : " << _minZ << '\n';
  //  std::cout << "Min X : " << _minX << '\n';
  //  std::cout << "Range Z : " << _rangeZ << '\n';
  //  std::cout << "Range X : " << _rangeX << '\n';
  //  std::cout << "Num Z : " << _numZ << '\n';
  //  std::cout << "Num X : " << _numX << '\n';
  //  std::cout << "TOTAL (Calculated): " << _numX*_numZ << '\n';
  //  std::cout << "TOTAL (Measured)  : " << pixels.count() << '\n';

    if ( pixels.count() != _numX*_numZ )
    {
      FAILURE_LOG( "Field Map Pixels do not Form a Square" );
      abort();
    }

    while ( pixels )
    {
      boolPixel< 2 > p = pixels.pop_front();
      map[ _pixelNum( p.position()[0], p.position()[1] ) ] = p;
    }

    this->setMap( map );
  }


  stdexts::dll< boolPixel< 2 > > cylindricalBool_map::_load( std::string filename, unsigned int colZ, unsigned int colX, unsigned int colLive ) const
  {
    stdexts::dll< boolPixel< 2 > > pixels;

    std::ifstream file( filename.c_str() );
    file.setf( std::ios_base::skipws );

    char line[ MAX_LINE_SIZE ];
    file.getline( line, MAX_LINE_SIZE );

    double tmp = 0;
    std::vector< double > row;
    unsigned long int count = 0;

    while ( file )
    {
      if ( ! stdexts::isComment( line ) )
      {
        std::istringstream ss( line );
        row.push_back( ++count );
        while ( 1 ) 
        {
          ss >> tmp;
          if ( ! ss ) break;
          row.push_back( tmp );
        }
        if ( row.size() < 3 )
        {
          FAILURE_LOG( "Not Enough Rows in Map File" );
          abort();
        }

        bool live = ( row[ colLive ] != 0 );

        nVector< 2 > pos = makeTwoVector( row[ colZ ], row[ colX ] );

        boolPixel< 2 > p( pos, live );

        pixels << p;
        row.clear();
      }
      file.getline( line, MAX_LINE_SIZE );
    }

    return pixels;
  }


  unsigned long int cylindricalBool_map::_pixelNum( double z, double x ) const
  {
    unsigned long int nZ = (unsigned long int)( ( (double)_numZ - 0.5 ) * ( ( z - _minZ ) / _rangeZ ) );
    unsigned long int nX = (unsigned long int)( ( (double)_numX - 0.5 ) * ( ( x - _minX ) / _rangeX ) );

    unsigned long int tmp = ( nX * _numZ ) + nZ;

    // std::cout << "Point : " << z << ", " << x << ". Num Z : " << nZ << ", Num X : " << nX << ". Val : " << tmp << '\n';
    return tmp;
  }


  bool cylindricalBool_map::live( threeVector vec ) const
  {
    threeVector pos = vec - this->center();

    double z = pos[2];
    double x = std::sqrt( pos[0]*pos[0] + pos[1]*pos[1] );

    if ( ( z > _maxZ ) || ( z < _minZ ) || ( x > _maxX ) || ( x < _minX ) )
    {
      return false;
    }

    return this->map()[ this->_pixelNum( z, x ) ].live();
  }


////////////////////////////////////////////////////////////////////////////////
// Sculpted Cylinder
////////////////////////////////////////////////////////////////////////////////

  sculptedCylinder::sculptedCylinder( cylindricalBool_map* map ) :
    cylinder( map->radius(), map->length(), map->center() ),
    _fieldMap( map )
  {
  }

  sculptedCylinder::sculptedCylinder( threeVector pos, rotation rot, double rad, double len, cylindricalBool_map* map ) :
    cylinder( rad, len, pos, rot ),
    _fieldMap( map )
  {
  }

  // print_base classes don't exist - left over from another library
//  print_base* sculptedCylinder::_print() const
//  {
//    print_primitive* newShape = new print_primitive( "Cylinder" );
//    
//    newShape->addParam( "radius",  stdexts::stringConvert( radius() ) );
//    newShape->addParam( "height",  stdexts::stringConvert( height() ) );
//    newShape->addParam( "side",  "TRUE" );
//    newShape->addParam( "bottom",  "TRUE" );
//    newShape->addParam( "top",  "TRUE" );
//    newShape->setPosition( this->center() );
//    newShape->setRotation( 1.0, 0.0, 0.0, 1.570796 );
//
//    return newShape;
//  }

}


