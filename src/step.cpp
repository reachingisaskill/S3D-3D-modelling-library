
#include "S3D_step.h"


namespace S3D
{

  step::step( threeVector pos, threeVector dir ) :
    _start( pos ),
    _direction( dir )
  {
  }

  step::step( const step& obj ) :
    _start( obj._start ),
    _direction( obj._direction )
  {
  }

  step& step::operator=( const step& obj )
  {
    this->_start = obj._start;
    this->_direction = obj._direction;

    return *this;
  }

  step::~step()
  {
  }


  threeVector step::getPosition() const
  {
    return _start + ( 0.5 * _direction );
  }

  void step::setPosition( threeVector pos )
  {
    _start = pos - ( 0.5 * _direction );
  }

  threeVector step::getEnd() const
  {
    return _start + _direction;
  }

  void step::setEnd( threeVector pos )
  {
    _start = pos - _direction;
  }


  bool step::operator==( const step& stp ) const
  {
    return ( this->_start == stp._start ) && ( this->_direction == stp._direction );
  }

}

