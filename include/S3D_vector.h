
#ifndef __S3D__VECTOR_H__
#define __S3D__VECTOR_H__

//#include "logtastic.h"

#include <cmath>
#include <ostream>


//////////////////////////////////////////////////////////////////////


// Forward Declarations

template < unsigned long int N, typename T >
class nVector;


template < typename T >
nVector< 2, T > makeTwoVector( T, T );

template < typename T >
nVector< 3, T > makeThreeVector( T, T, T );

template < typename T >
nVector< 4, T > makeFourVector( T, T, T, T );


//////////////////////////////////////////////////////////////////////


typedef nVector< 2, double > twoVector;
typedef nVector< 3, double > threeVector;
typedef nVector< 4, double > fourVector;


//////////////////////////////////////////////////////////////////////


template < unsigned long int N, typename T = double >
class nVector
{
  friend nVector< 2, T > makeTwoVector<>( T, T );
  friend nVector< 3, T > makeThreeVector<>( T, T, T );
  friend nVector< 4, T > makeFourVector<>( T, T, T, T );

  private:
    T _components[N];

  public:
    explicit nVector();
    explicit nVector( T );
    explicit nVector( T* );
    nVector( const nVector<N, T>& );
    nVector< N, T >& operator= ( const nVector< N, T >& );
    nVector< N, T >& operator= ( const T& );
    ~nVector();

    T& get( unsigned long int n );
    const T& get( unsigned long int n ) const;
    T& operator[] ( unsigned long int n ) { return get(n); }
    const T& operator[] ( unsigned long int n ) const { return get(n); }

    template < unsigned long int C >
    nVector< C, T >& getSlice( unsigned long int start ) const;

    unsigned long int length() const { return N; }
    double square() const;
    double mod() const { return sqrt( square() ); }
    nVector< N, T > norm() const;



    virtual nVector< N, T > operator* ( T ) const; // Multiply by constant
    virtual T operator*( const nVector< N, T >& ) const; // Dot Product
    virtual nVector< N, T > operator+ ( const nVector< N, T >& ) const;
    virtual nVector< N, T > operator- ( const nVector< N, T >& ) const; 
    virtual nVector< N, T > operator- () const; 
    virtual nVector< N, T >& operator+= ( const nVector< N, T >& );
    virtual nVector< N, T >& operator-= ( const nVector< N, T >& );
    virtual bool operator== ( const nVector< N , T >& ) const;
    virtual bool operator!= ( const nVector< N , T >& ) const;


//    template < unsigned long int NN, typename TT >
//    friend T operator* ( nVector< NN, TT >, nVector< NN, TT > );

    template < unsigned long NN, typename TT >
    friend nVector< NN, TT > operator* ( TT, nVector< NN, TT > ); // Pre-Mulitply by constant
    
    template < typename TT >
    friend nVector< 3, TT > operator^ ( nVector< 3, TT >, nVector< 3, TT > ); // Cross Product

    template < unsigned long int NN, typename TT >
    friend nVector< NN+1, TT > operator, ( TT, nVector< NN, TT > );
    template < unsigned long int NN, typename TT >
    friend nVector< NN+1, TT > operator, ( nVector< NN, TT >, TT );
    template < unsigned long int NN, unsigned long int MM, typename TT >
    friend nVector< NN+MM, TT > operator, ( nVector< NN, TT >, nVector< MM, TT > );

    template < unsigned long int NN, typename TT >
    friend std::ostream& operator<< ( std::ostream&, nVector< NN, TT > );

    template < unsigned long int NN, typename TT >
    friend class nVector;

};

//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////


//template < unsigned long int N, typename T >
//nVector< N, T >::nVector() :
//  _components( new T[ N ] )
//{
//}
//
//template < unsigned long int N, typename T >
//nVector< N, T >::nVector( T initVal ) :
//  _components( new T[ N ] )
//{
//  for ( unsigned long int i = 0; i < N; ++i )
//    _components[i] = initVal;
//}
//
//template < unsigned long int N, typename T >
//nVector< N, T >::nVector( T* array ) :
//  _components( new T[ N ] )
//{
//  for ( int i = 0; i < N; ++i )
//    _components[i] = array[i];
//}
//
//
//template < unsigned long int N, typename T >
//nVector< N, T >::nVector( const nVector<N, T>& vec ) :
//  _components( new T[ N ] )
//{
//  for ( unsigned long int i = 0; i < N; ++i )
//  {
//    T temp = vec._components[i];
//    this->_components[i] = temp;
//  }
//}


template < unsigned long int N, typename T >
nVector< N, T >::nVector() :
  _components()
{
}

template < unsigned long int N, typename T >
nVector< N, T >::nVector( T initVal ) :
  _components()
{
  for ( unsigned long int i = 0; i < N; ++i )
    _components[i] = initVal;
}

template < unsigned long int N, typename T >
nVector< N, T >::nVector( T* array ) :
  _components()
{
  for ( int i = 0; i < N; ++i )
    _components[i] = array[i];
}


template < unsigned long int N, typename T >
nVector< N, T >::nVector( const nVector<N, T>& vec ) :
  _components()
{
  for ( unsigned long int i = 0; i < N; ++i )
  {
    T temp = vec._components[i];
    this->_components[i] = temp;
  }
}

template < unsigned long int N, typename T >
nVector< N, T >& nVector< N, T >::operator= ( const nVector< N, T >& vec )
{
  for ( unsigned long int i = 0; i < N; ++i )
    this->_components[i] = vec._components[i];
  return *this;
}

template < unsigned long int N, typename T >
nVector< N, T >& nVector< N, T >::operator= ( const T& c )
{
  for ( unsigned long int i = 0; i < N; ++i )
    this->_components[i] = c;
  return *this;
}

template < unsigned long int N, typename T >
bool nVector< N, T >::operator== ( const nVector< N , T >& v ) const
{
  for ( unsigned long int i = 0; i < N; ++i )
    if ( v._components[i] != this->_components[i] ) return false;

  return true;
}

template < unsigned long int N, typename T >
bool nVector< N, T >::operator!= ( const nVector< N , T >& v ) const
{
  for ( unsigned long int i = 0; i < N; ++i )
    if ( v._components[i] == this->_components[i] ) return false;

  return true;
}

template < unsigned long int N, typename T >
nVector< N, T > nVector< N, T >::operator-() const
{
  nVector newVec;
  for ( unsigned long int i = 0; i < N; ++i )
    newVec._components[i] = ( -this->_components[i] );
  
  return newVec;
}


template < unsigned long int N, typename T >
nVector< N, T >::~nVector()
{
//  delete [] _components;
}


template < unsigned long int N, typename T >
T& nVector< N, T >::get( unsigned long int n )
{
  if ( n >= N )
  {
    // ERROR_LOG( "Attempting to access nVector outside bounds" );
    return _components[N-1];
  }
  return _components[n];
}

template < unsigned long int N, typename T >
const T& nVector< N, T >::get( unsigned long int n ) const
{
  if ( n >= N )
  {
    // ERROR_LOG( "Attempting to access nVector outside bounds" );
    return _components[N-1];
  }
  return _components[n];
}

template < unsigned long int N, typename T >
template < unsigned long int C >
nVector< C, T >& nVector< N, T >::getSlice( unsigned long int start ) const
{
  static nVector< C, T > retVec;
//  if ( start > N ) ERROR_LOG( "Attempting to access nVector outside bounds");
  
  for ( unsigned long int i = start; i < N; ++i )
  {
    retVec._components[i - start] = _components[i];
  }

  return retVec;
}

template < unsigned long int N, typename T >
nVector< N, T > nVector< N, T >::operator* ( T value ) const
{
  nVector< N, T > newVec;

  for ( unsigned long int i = 0; i < N; ++i )
  {
    newVec[i] = _components[i] * value;
  }

  return newVec;
}

template < unsigned long int N, typename T >
T nVector< N, T >::operator* ( const nVector< N, T >& vec ) const
{
  T retVal = (T)0;
  for ( unsigned long int i = 0; i < N; ++i )
  {
    retVal += _components[i] * vec._components[i];
  }
  return retVal;
}

template < unsigned long int N, typename T >
nVector< N, T > nVector< N, T >::operator+ ( const nVector< N, T >& vec ) const
{
  nVector< N, T > newVec;

  for ( unsigned int i = 0; i < N; ++i )
  {
    newVec[i] = _components[i] + vec[i];
  }

  return newVec;
}

template < unsigned long int N, typename T >
nVector< N, T > nVector< N, T >::operator- ( const nVector< N, T >& vec ) const
{
  nVector< N, T > newVec;

  for ( unsigned int i = 0; i < N; ++i )
  {
    newVec[i] = _components[i] - vec[i];
  }

  return newVec;
}

template < unsigned long int N, typename T >
nVector< N, T >& nVector< N, T >::operator+= ( const nVector< N, T >& vec )
{
  for ( unsigned int i = 0; i < N; ++i )
  {
    _components[i] += vec[i];
  }

  return *this;
}

template < unsigned long int N, typename T >
nVector< N, T >& nVector< N, T >::operator-= ( const nVector< N, T >& vec )
{
  for ( unsigned int i = 0; i < N; ++i )
  {
    _components[i] -= vec[i];
  }

  return *this;
}

template < unsigned long int N, typename T >
double nVector< N, T >::square() const
{
  double sum = 0.0;
  for ( unsigned long int i = 0; i < N; ++i )
  {
    sum += _components[i] * _components[i];
  }
  return sum;
}

template < unsigned long int N, typename T >
nVector< N, T > nVector< N, T >::norm() const
{
  return (*this) * ( 1.0 / this->mod() );
}

//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////


template< unsigned long int N, typename T >
nVector< N, T > operator* ( T c, nVector< N, T > vec )
{
  nVector< N, T > newVec;
  for ( unsigned long int i = 0; i < N; ++i )
    newVec[i] = vec[i] * c;

  return newVec;
}

template < typename T >
nVector< 3, T > operator^ ( nVector< 3, T> v1, nVector< 3, T> v2 )
{
  nVector< 3, T > result;
  result._components[0] = v1._components[1]*v2._components[2] - v1._components[2]*v2._components[1];
  result._components[1] = v1._components[2]*v2._components[0] - v1._components[0]*v2._components[2];
  result._components[2] = v1._components[0]*v2._components[1] - v1._components[1]*v2._components[0];

  return result;
}

template < unsigned long int NN, typename TT >
nVector< NN+1, TT > operator, ( TT val, nVector< NN, TT > vec )
{
  nVector< NN+1, TT > retVec( val );
  for ( unsigned long int i = 0; i < NN; ++i )
    retVec._components[ i+1 ] = vec._components[ i ];
  return retVec;
}

template < unsigned long int NN, typename TT >
nVector< NN+1, TT > operator, ( nVector< NN, TT > vec, TT val )
{
  nVector< NN+1, TT > retVec( val );
  for ( unsigned long int i = 0; i < NN; ++i )
    retVec._components[ i ] = vec._components[ i ];
  return retVec;
}

template < unsigned long int NN, unsigned long int MM, typename TT >
nVector< NN+MM, TT > operator, ( nVector< NN, TT > vec1, nVector< MM, TT > vec2 )
{
  nVector< NN+MM, TT > retVec;
  unsigned long int i = 0;
  for (  i = 0; i < NN; ++i )
    retVec._components[ i ] = vec1._components[ i ];
  for ( ; i < NN+MM; ++i ) 
    retVec._components[ i ] = vec2._components[ i-NN ];

  return retVec;
}

template < unsigned long int NN, typename TT >
std::ostream& operator<< ( std::ostream& out, nVector< NN, TT > vec )
{
  for ( unsigned long int i = 0; i < NN; ++ i )
  {
    out << vec._components[i] << ' ';
  }
  return out;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

  template < typename T >
  inline nVector< 2, T > makeTwoVector( T a, T b )
  {
    nVector< 2, T > temp;
    temp._components[0] = a;
    temp._components[1] = b;
    return temp;
  }

  template < typename T >
  inline nVector< 3, T > makeThreeVector( T a, T b, T c )
  {
    nVector< 3, T > temp;
    temp._components[0] = a;
    temp._components[1] = b;
    temp._components[2] = c;
    return temp;
  }

  template < typename T >
  inline nVector< 4, T > makeFourVector( T a, T b, T c, T d )
  {
    nVector< 4, T > temp;
    temp._components[0] = a;
    temp._components[1] = b;
    temp._components[2] = c;
    temp._components[2] = d;
    return temp;
  }


//////////////////////////////////////////////////////////////////////

#endif // __S3D__VECTOR_H__

