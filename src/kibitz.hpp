#ifndef __KIBITZ_HPP__
#define __KIBITZ_HPP__

#include "common.hpp"



namespace kibitz {

  typedef void ( *callback )( const string&  );

  void initialize(const variables_map& application_configuration ) ;
  void terminate();

}

#endif
