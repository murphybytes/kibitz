#ifndef __HEARTBEAT_SENDER_HPP__
#define __HEARTBEAT_SENDER_HPP__

#include "message_base.hpp"


namespace kibitz {

  class heartbeat_sender : public message_base {

  public:
    heartbeat_sender( context* context ) ; 

    ~heartbeat_sender();  
    void operator()();
  };



}



#endif
