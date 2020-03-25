
#include "S3D_primitives.h"

#include "S3D_defs.h"

#include "logtastic.h"
#include <cassert>


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

    if ( ( this->getPosition() - l.getStart() ) * l.getDirection() <= 0.0 ) // Going backwards
    {
      return false;
    }

    double dist = l.distance( this->getPosition() );
    if ( dist <= 0.0 ) return false;
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
    double ad = alpha * l.getDirection().norm(); // Dot product
    double a_sq = alpha * alpha;
    double r_sq = this->_radius*this->_radius;

    double distance = -ad - std::sqrt( ad*ad + r_sq - a_sq );

    point thePoint = l.getStart() + distance*l.getDirection();

    return interaction( thePoint, &l, (object_base*) this, ( thePoint - this->getPosition() ).norm() );
  }


////////////////////////////////////////////////////////////////////////////////////////////////////
  // Box

  box::box( material_base* mat, double x, double y, double z ) :
    object_base( mat ),
    _lengthX( x ),
    _lengthY( y ),
    _lengthZ( z ),
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

    for ( unsigned int i = 0; i < 6; ++i )
      _surfaces[i].rotateAbout( this->getRotation(), this->getPosition() );
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

    if ( std::fabs(2.0*sep[0]) > _lengthX )
      return false;
    else if ( std::fabs(2.0*sep[1]) > _lengthY )
      return false;
    else if ( std::fabs(2.0*sep[2]) > _lengthZ )
      return false;
    else
      return true;
  }


  bool box::crosses( const line& l ) const
  {
    if ( this->contains( l.getStart() ) )
    {
      return true;
    }
    else
    {
      for ( unsigned int i = 0; i < 6; ++i )
      {
        if ( _surfaces[i].crosses( l ) )
          return true;
      }
      return false;
    }
  }


  interaction box::intersect( const line& l ) const
  {
    point current_point;
    const surface_rectangle* current_surface = nullptr;
    double current_distance = 1.0e20;

    INFO_LOG( "Calculating Box Intersect" );
    for ( unsigned int i = 0; i < 6; ++i )
    {
      if ( _surfaces[i].crosses( l ) )
      {
        point test = _surfaces[i].intersect( l );
        double distance = ( test - l.getStart() ) * l.getDirection();

        if ( distance < 0.0 ) continue; // No going backwards!

        INFO_STREAM << "Test: " << test.getPosition() << " -- Dist = " << distance;
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
      FAILURE_LOG( "Line does not cross surface (box::intersect(...))" );
      FAILURE_STREAM << "Line: " << l.getStart().getPosition() << " -> " << l.getDirection();
      assert(false);
    }

    return interaction( current_point, &l, (object_base*) this, current_surface->getNormal() );
  }



//  double box::distance( const point& p ) const
//  {
//    if ( this->contains( p ) ) return 0.0;
//
//    // Count how many surfaces the point is infront of.
//    unsigned int count = 0;
//    bool[6] fronts = { false };
//    surface *theSurfaces[3];
//
//    for ( unsigned int i = 0; i < 6; ++i )
//    {
//      if ( _surfaces[i].inFront( p ) )
//      {
//        fronts[i] = true;
//        theSurfaces[count] = &_surfaces[i];
//        count += 1;
//      }
//      else
//        fronts[i] = false;
//    }
//
//    // If only one, return the surface-point distance.
//    if ( count == 1 )
//    {
//      for ( unsigned int i = 0; i < 6; ++i )
//      {
//        if ( fronts[i] )
//        return _surfaces[i].distance( p );
//      }
//    }
//
//    // Find the closest edge.
//    line edge = surfaceIntersection( *theSurfaces[0], *theSurfaces[1] );
//    if ( count == 2 )
//    {
//      return edge.distance( p );
//    }
//    else if ( count == 3 )
//    {
//      // Find the closests vertex.
//      point vertex = theSurfaces[2]->crosses( edge );
//      return ( p - vertex ).mod();
//    }
//    else
//    {
//      FAIL_LOG( "Point is in front of too many surfaces, I broke maths..." );
//      assert( count < 4 );
//    }
//  }
}

