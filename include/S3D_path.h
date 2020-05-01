
#ifndef _S3D_PATH_H_
#define _S3D_PATH_H_

#include "S3D_interaction.h"
#include "S3D_spectrum.h"

#include "stdexts.h"

#include <vector>


namespace S3D
{

  class pathvertex
  {
    private:
      interaction _theInteraction;
      spectrum _throughput;
      double _weight;

    public:
      pathvertex();
      pathvertex( interaction, spectrum, double weight = 1.0 );

      interaction getInteraction() const { return _theInteraction; }
      spectrum getThroughput() const { return _throughput; }
      double getWeight() const { return _weight; }
  };


  typedef std::vector< pathvertex > path;

//
//  class path
//  {
//    private:
//      std::vector< pathvertex > _thePath;
//
//    public:
//      path();
//
//      void push_back( pathvertex );
//
//      spectrum getThroughput() const { return _thePath.rbegin()->getThroughput(); }
//
//      pathvertex& last() { return *_thePath.rbegin(); }
//
//      std::size_t size() const { return _thePath.size(); }
//
//      bool empty() const { return _thePath.size() == 0; }
//
//      pathvertex& operator[]( size_t i ) { return _thePath[i]; }
//  };

}

#endif // _S3D_PATH_H_

