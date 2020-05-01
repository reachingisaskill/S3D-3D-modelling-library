#define LOGTASTIC_DEBUG_OFF

#include "S3D_primitives.h"

#include "S3D_defs.h"
#include "S3D_manager.h"
#include "S3D_random.h"

#include "logtastic.h"

#include <cassert>
#include <sstream>


namespace S3D
{

////////////////////////////////////////////////////////////////////////////////////////////////////
  // Sphere

  sphere::sphere( material_base* mat, double radius ) :
    object_base( mat ),
    _radius( radius )
  {
  }


  bool sphere::contains( const point& p ) const
  {
    if ( ( this->getPosition() - p ).mod() <= this->getRadius() )
      return true;
    else
      return false;
  }


  bool sphere::crosses( const line& l ) const
  {
    if ( this->contains( l.getStart() ) )
    {
      return true;
    }

    if ( ( this->getPosition() - l.getStart() ) * l.getDirection() <= epsilon ) // Going backwards / self interaction
    {
      return false;
    }

    // Calculate the perpendicular distance of the line from the centre.
    double dist = l.distance( this->getPosition() );
    return ( dist <= this->getRadius() );
  }


  double sphere::distance( const line& l ) const
  {
    return ( l.distance( this->getPosition() ) - this->getRadius() );
  }


  double sphere::distance( const point& p ) const
  {
    return ( ( p - this->getPosition() ).mod() - this->getRadius() );
  }


  interaction sphere::intersect( const line& l ) const
  {
    threeVector alpha = l.getStart() - this->getPosition();
    double ad = -alpha * l.getDirection(); // Dot product
    double a_sq = alpha * alpha;
    double r_sq = this->_radius*this->_radius;

    double root = std::sqrt( ad*ad + r_sq - a_sq );
//    if ( ad > root ) // Is the interaction point outside
    if ( ( ad - root ) > epsilon ) // Is the interaction point outside
    {
      double distance = ad - root ;
      point thePoint = l.getStart() + distance*l.getDirection();
      threeVector normal = ( thePoint - this->getPosition() ).norm();
      double indexRatio = manager::getInstance()->getWorld()->getMaterial()->getRefractiveIndex() / this->getMaterial()->getRefractiveIndex();
      return interaction( thePoint, &l, (object_base*) this, normal, indexRatio  );
    }
    else // or inside?
    {
      double distance = ad + root ;
      point thePoint = l.getStart() + distance*l.getDirection();
      threeVector normal = ( this->getPosition() - thePoint ).norm();
      double indexRatio = this->getMaterial()->getRefractiveIndex() / manager::getInstance()->getWorld()->getMaterial()->getRefractiveIndex();
      return interaction( thePoint, &l, (object_base*) this, normal, indexRatio );
    }

  }


  surfacemap sphere::sampleSurface() const
  {
    double r1 = random::uniformDouble();
    double r2 = random::uniformDouble();

    double theta = r1*2.0*PI;
    double phi = std::acos( 2.0* r2 - 1 );

    threeVector pos = makeThreeVector( std::cos(theta)*std::sin(phi), std::sin(theta)*std::sin(phi), std::cos(phi) );
    return surfacemap( this->getPosition() + pos, pos.norm() );
  }


  line sphere::sampleEmission() const
  {
    double r1 = random::uniformDouble();
    double r2 = random::uniformDouble();

    double theta = r1*2.0*PI;
    double phi = std::acos( 2.0* r2 - 1 );

//    rotation rot = rotation( unit_threeVector_z, theta ) * rotation( unit_threeVector_x, phi );
    threeVector pos = makeThreeVector( std::cos(theta)*std::sin(phi), std::sin(theta)*std::sin(phi), std::cos(phi) );
    rotation rot( crossProduct( defaultDirection, pos ), vectorAngle( defaultDirection, pos ) );
    return line( this->getPosition() + _radius*pos, random::uniformHemisphere( rot ) );
  }


////////////////////////////////////////////////////////////////////////////////////////////////////
  // Box

  box::box( material_base* mat, double x, double y, double z ) :
    object_base( mat ),
    _lengthX( x ),
    _lengthY( y ),
    _lengthZ( z ),
    _areaSum( 0.0 ),
    _surfaces()
  {
    _makeSurfaces();
  }


  void box::_makeSurfaces()
  {
    INFO_LOG( "Building box surfaces." );
    point tmp = this->getPosition();
    tmp[0] += 0.5*_lengthX;
    _surfaces[0] = surface_rectangle( _lengthZ, _lengthY );
    _surfaces[0].setPosition( tmp );
    _surfaces[0].setRotation( rotation( unit_threeVector_y, 0.5*PI ) );
    tmp[0] -= _lengthX;
    _surfaces[1] = surface_rectangle( _lengthZ, _lengthY );
    _surfaces[1].setPosition( tmp );
    _surfaces[1].setRotation( rotation( unit_threeVector_y, -0.5*PI ) );

    tmp = getPosition();
    tmp[1] += 0.5*_lengthY;
    _surfaces[2] = surface_rectangle( _lengthX, _lengthZ );
    _surfaces[2].setPosition( tmp );
    _surfaces[2].setRotation( rotation( unit_threeVector_x, -0.5*PI ) );
    tmp[1] -= _lengthY;
    _surfaces[3] = surface_rectangle( _lengthX, _lengthZ );
    _surfaces[3].setPosition( tmp );
    _surfaces[3].setRotation( rotation( unit_threeVector_x, 0.5*PI ) );

    tmp = getPosition();
    tmp[2] += 0.5*_lengthZ;
    _surfaces[4] = surface_rectangle( _lengthX, _lengthY );
    _surfaces[4].setPosition( tmp );
    _surfaces[4].setRotation( rotation() );
    tmp[2] -= _lengthZ;
    _surfaces[5] = surface_rectangle( _lengthX, _lengthY );
    _surfaces[5].setPosition( tmp );
    _surfaces[5].setRotation( rotation( unit_threeVector_y, PI ) );

    _areaSum = 0.0;
    for ( unsigned int i = 0; i < 6; ++i )
    {
      _surfaces[i].rotateAbout( this->getRotation(), this->getPosition() );
      _areaSum += _surfaces[i].getArea();
    }
  }


  void box::setPosition( point pos )
  {
    base::setPosition( pos );
    this->_makeSurfaces();
  }


  void box::setRotation( rotation rot )
  {
    base::setRotation( rot );
    this->_makeSurfaces();
  }


  void box::rotate( rotation rot )
  {
    base::rotate( rot );
    this->_makeSurfaces();
  }


  void box::rotateAbout( rotation rot, point pos )
  {
    base::rotateAbout( rot, pos );
    this->_makeSurfaces();
  }


  bool box::contains( const point& p ) const
  {
    threeVector sep = p - this->getPosition();
    threeVector rotated_sep = this->getRotation() / sep;

    if ( std::fabs(2.0*rotated_sep[0]) > _lengthX )
      return false;
    else if ( std::fabs(2.0*rotated_sep[1]) > _lengthY )
      return false;
    else if ( std::fabs(2.0*rotated_sep[2]) > _lengthZ )
      return false;
    else
      return true;
  }


  bool box::crosses( const line& l ) const
  {
    for ( unsigned int i = 0; i < 6; ++i )
    {
      if ( _surfaces[i].crosses( l ) )
        return true;
    }
    return false;
  }


  interaction box::intersect( const line& l ) const
  {
    point current_point;
    const surface_rectangle* current_surface = nullptr;
    double current_distance = 1.0e20;
    DEBUG_STREAM << "box::intersect() : Line = " << l.getStart().getPosition() << " -- " << l.getDirection();

    for ( unsigned int i = 0; i < 6; ++i )
    {
      if ( _surfaces[i].crosses( l ) )
      {
        point test = _surfaces[i].intersect( l );
        double distance = ( test - l.getStart() ) * l.getDirection();

//        if ( distance < 0.0 ) continue; // No going backwards!
        DEBUG_STREAM << "Testing distance: " << distance << ", " << current_distance;
        if ( ( distance - current_distance ) < epsilon )
        {
          current_point = test;
          current_distance = distance;
          current_surface = &_surfaces[i];
        }
      }
    }

    if ( current_surface == nullptr )
    {
      std::stringstream ss;
      ss << "Line: " << l.getStart().getPosition() << " -> " << l.getDirection();
      stdexts::exception ex( "Intersection of line with box surface could not be calculated.", ss.str() );
      FAILURE_LOG( "Line does not cross surface (box::intersect(...))" );
      FAILURE_LOG( ss.str().c_str() );

      EX_CREATE( ex );
      THROW( ex );
    }

    if ( ( current_point - l.getStart() ) * current_surface->getNormal() < 0.0 ) // Line pointing inwards
    {
      threeVector normal = current_surface->getNormal();
      double indexRatio = manager::getInstance()->getWorld()->getMaterial()->getRefractiveIndex() / this->getMaterial()->getRefractiveIndex();
      return interaction( current_point, &l, (object_base*) this, normal, indexRatio );
    }
    else // Line pointing outwards
    {
      threeVector normal = -current_surface->getNormal();
      double indexRatio = this->getMaterial()->getRefractiveIndex() / manager::getInstance()->getWorld()->getMaterial()->getRefractiveIndex();
      return interaction( current_point, &l, (object_base*) this, normal, indexRatio );
    }
  }


  surfacemap box::sampleSurface() const
  {
    // Pick a random side of box, weighted by each surface area.
    double randomArea = _areaSum*random::uniformDouble();
    int planeId = 5;
    double sum = 0.0;
    for ( unsigned int i = 0; i < 5; ++i ) // Don't need to test for the 6th surface.
    {
      sum += _surfaces[i].getArea();
      if ( randomArea < sum )
      {
        planeId = i;
        break;
      }
    }

    // Pick a random position on that plane
    return surfacemap( _surfaces[planeId].sampleSurface(), _surfaces[planeId].getNormal() );
  }


  line box::sampleEmission() const
  {
    // Pick a random side of box, weighted by each surface area.
    double randomArea = _areaSum*random::uniformDouble();
    int planeId = 5;
    double sum = 0.0;
    for ( unsigned int i = 0; i < 5; ++i ) // Don't need to test for the 6th surface.
    {
      sum += _surfaces[i].getArea();
      if ( randomArea < sum )
      {
        planeId = i;
        break;
      }
    }

    // Pick a random position on that plane
    point pos = _surfaces[planeId].sampleSurface();

    // Pick a random direction
    threeVector dir = random::uniformHemisphere( _surfaces[planeId].getRotation() );

    return line( pos, dir );
  }


////////////////////////////////////////////////////////////////////////////////////////////////////
  // Square Plane

  square_plane::square_plane( material_base* mat, double x, double y ) :
    object_base( mat ),
    _surface( x, y )
  {
  }


  void square_plane::_makeSurface()
  {
    _surface.setPosition( this->getPosition() );
    _surface.setRotation( this->getRotation() );
  }


  void square_plane::setPosition( point pos )
  {
    base::setPosition( pos );
    this->_makeSurface();
  }


  void square_plane::setRotation( rotation rot )
  {
    base::setRotation( rot );
    this->_makeSurface();
  }


  void square_plane::rotate( rotation rot )
  {
    base::rotate( rot );
    this->_makeSurface();
  }


  void square_plane::rotateAbout( rotation rot, point pos )
  {
    base::rotateAbout( rot, pos );
    this->_makeSurface();
  }


  bool square_plane::contains( const point& p ) const
  {
    return ! _surface.inFront( p );
  }


  bool square_plane::crosses( const line& l ) const
  {
    return _surface.crosses( l );
  }

  interaction square_plane::intersect( const line& l ) const
  {
    point inter = _surface.intersect( l );
    if ( _surface.inFront( l.getStart() ) )
    {
      threeVector normal = _surface.getNormal();
      double indexRatio = manager::getInstance()->getWorld()->getMaterial()->getRefractiveIndex() / this->getMaterial()->getRefractiveIndex();
      return interaction( inter, &l, (object_base*) this, normal, indexRatio );
    }
    else // Line from behind => reverse the normal
    {
      threeVector normal = -_surface.getNormal();
      double indexRatio = this->getMaterial()->getRefractiveIndex() / manager::getInstance()->getWorld()->getMaterial()->getRefractiveIndex();
      return interaction( inter, &l, (object_base*) this, normal, indexRatio );
    }
  }

  
  surfacemap square_plane::sampleSurface() const
  {
    point pos =  _surface.sampleSurface();
    return surfacemap( pos, _surface.getNormal() );
  }

  
  line square_plane::sampleEmission() const
  {
    point pos =  _surface.sampleSurface();
    threeVector dir =  random::uniformHemisphere( _surface.getRotation() );
    return line( pos, dir );
  }


////////////////////////////////////////////////////////////////////////////////////////////////////
  // Circular Plane

  circular_plane::circular_plane( material_base* mat, double r ) :
    object_base( mat ),
    _surface( r )
  {
  }


  void circular_plane::_makeSurface()
  {
    _surface.setPosition( this->getPosition() );
    _surface.setRotation( this->getRotation() );
  }


  void circular_plane::setPosition( point pos )
  {
    base::setPosition( pos );
    this->_makeSurface();
  }


  void circular_plane::setRotation( rotation rot )
  {
    base::setRotation( rot );
    this->_makeSurface();
  }


  void circular_plane::rotate( rotation rot )
  {
    base::rotate( rot );
    this->_makeSurface();
  }


  void circular_plane::rotateAbout( rotation rot, point pos )
  {
    base::rotateAbout( rot, pos );
    this->_makeSurface();
  }


  bool circular_plane::contains( const point& p ) const
  {
    return ! _surface.inFront( p );
  }


  bool circular_plane::crosses( const line& l ) const
  {
    return _surface.crosses( l );
  }

  interaction circular_plane::intersect( const line& l ) const
  {
    point inter = _surface.intersect( l );
    if ( _surface.inFront( l.getStart() ) )
    {
      threeVector normal = _surface.getNormal();
      double indexRatio = manager::getInstance()->getWorld()->getMaterial()->getRefractiveIndex() / this->getMaterial()->getRefractiveIndex();
      return interaction( inter, &l, (object_base*) this, normal, indexRatio );
    }
    else // Line from behind => reverse the normal
    {
      threeVector normal = -_surface.getNormal();
      double indexRatio = this->getMaterial()->getRefractiveIndex() / manager::getInstance()->getWorld()->getMaterial()->getRefractiveIndex();
      return interaction( inter, &l, (object_base*) this, normal, indexRatio );
    }
  }

  
  surfacemap circular_plane::sampleSurface() const
  {
    point pos =  _surface.sampleSurface();
    return surfacemap( pos, _surface.getNormal() );
  }

  
  line circular_plane::sampleEmission() const
  {
    point pos =  _surface.sampleSurface();
    threeVector dir =  random::uniformHemisphere( _surface.getRotation() );
    return line( pos, dir );
  }

}

