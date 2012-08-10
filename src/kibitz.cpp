#include "kibitz.hpp"

namespace kibitz {
  context& get_context() {
    static context* p = NULL;
    if( !p ) {
      p = new context();
    }
    return *p;
  }

  context::context() {
    std::cout << "ctor" << std::endl;
  }
}

