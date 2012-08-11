#ifndef __MESSAGE_BASE_HPP__
#define __MESSAGE_BASE_HPP__

#include "kibitz.hpp"
#include "context.hpp"

namespace kibitz {

  static const char* INPROC_COMMAND_BINDING = "inproc://#1";

  class message_base {

  protected:
    context_ptr context_;
    shared_ptr<boost::thread> thread_;
    void internal_command_handler(   );



  
  public:
    message_base( context_ptr context ) ;
    ~message_base(); 

  
  };

}

#endif
