
#ifndef __S3D__FIELD_MAP_H__
#define __S3D__FIELD_MAP_H__


#include <string>


namespace S3D
{

  template < unsigned int DIM >
  class boolPixel
  {
    private:
      nVector< DIM > _pos;
      bool _live;
      
    public:
      boolPixel() : _pos( 0.0 ), _live( false ) {}
      boolPixel( nVector< DIM > v, bool l ) : _pos( v ), _live( l ) {}

      virtual ~boolPixel() {}

      virtual const nVector< DIM >& position() const { return _pos; }
      virtual void position( nVector< DIM > p ) { _pos = p; }
      virtual const bool& live() const { return _live; }
      virtual void live( bool l ) { _live = l; }
  };


  template < unsigned int DIM, unsigned int FIELD >
  class fieldPixel : public boolPixel< DIM >
  {
    private:
      nVector< FIELD > _data;

    public:
      fieldPixel() : boolPixel< DIM >(), _data( 0.0 ) {}
      fieldPixel( nVector< DIM > p, bool l, nVector< FIELD > val ) : boolPixel< DIM >( p, l ), _data( val ) {}

      virtual ~fieldPixel() {}

      virtual const nVector< FIELD >& value() const { return _data; }
      virtual void value( nVector< FIELD > v ) { _data = v; }
  };


////////////////////////////////////////////////////////////////////////////////
// Map Classes
////////////////////////////////////////////////////////////////////////////////


  template < class T, class R >
  class map_base : public stdexts::non_copyable
  {
    private:
      threeVector _center;
      std::string _fileName;
      stdexts::sharedPtr< T[] > _map;

    protected:
      // Only set it once!
      void setMap( T* map ) { if ( ! _map ) _map = map; }
      const T* map() const { return _map.get(); }

    public:
      map_base( threeVector, std::string );

      virtual const std::string& filename() const { return _fileName; }

      virtual R value( threeVector ) const = 0;

      virtual bool live( threeVector ) const = 0;

      virtual threeVector center() const { return _center; }
  };


  template < class T >
  class scalarFieldMap : public map_base< T, double >
  {
    private:
    public:
      scalarFieldMap( threeVector, std::string );
      virtual ~scalarFieldMap() {}
  };


  template < class T >
  class vectorFieldMap : public map_base< T, threeVector >
  {
    private:
    public:
      vectorFieldMap( threeVector, std::string );
      virtual ~vectorFieldMap() {}
  };

  template < class T >
  class boolFieldMap : public map_base< T, bool >
  {
    private:
    public:
      boolFieldMap( threeVector, std::string );
      virtual ~boolFieldMap() {}
  };



////////////////////////////////////////////////////////////////////////////////


  template < class T, class R >
  map_base< T, R >::map_base( threeVector c, std::string fn ) :
    _center( c ),
    _fileName( fn ),
    _map( 0 )
  {
  }


  template < class T >
  scalarFieldMap< T >::scalarFieldMap( threeVector c, std::string fn ) :
    map_base< T, double >( c, fn )
  {
  }


  template < class T >
  vectorFieldMap< T >::vectorFieldMap( threeVector c, std::string fn ) :
    map_base< T, threeVector >( c, fn )
  {
  }


  template < class T >
  boolFieldMap< T >::boolFieldMap( threeVector c, std::string fn ) :
    map_base< T, bool >( c, fn )
  {
  }



}

#endif // __FES__FIELD_MAP_H__

