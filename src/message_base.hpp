#ifndef __MESSAGE_BASE_HPP__
#define __MESSAGE_BASE_HPP__

#include "kibitz.hpp"
#include "context.hpp"

namespace kibitz {


  class message_base {


  protected:
    bool* shutdown_;
    context* context_;


    

  
  public:
    message_base( context* context ) ;
    ~message_base(); 

    bool shutdown() const ;
    void internal_command_handler(   );  
  };

}

#endif
