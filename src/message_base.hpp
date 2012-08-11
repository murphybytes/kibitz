#ifndef __MESSAGE_BASE_HPP__
#define __MESSAGE_BASE_HPP__

#include "kibitz.hpp"
#include "context.hpp"

namespace kibitz {

class message_base {
protected:
  context_ptr context_;
public:
  message_base( context_ptr context ) ;
  ~message_base(); 

  
};

}

#endif
