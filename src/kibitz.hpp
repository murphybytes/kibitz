#ifndef __KIBITZ_HPP__
#define __KIBITZ_HPP__

#include <iostream>

namespace kibitz {

  class context {
    friend context& get_context();

    context();
  };

  context& get_context() ; 

}

#endif
