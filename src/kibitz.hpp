#ifndef __KIBITZ_HPP__
#define __KIBITZ_HPP__

#include "common.hpp"



namespace kibitz {

  typedef void ( *callback )( const string&  );

  void initialize( int argc, char* argv[] ) ; 
  void terminate();

}

#endif
