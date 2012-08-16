#ifndef heartbeat_receiver_hpp
#define heartbeat_receiver_hpp

#include "message_base.hpp"

namespace kibitz {
  class heartbeat_receiver : public message_base {
  public :
    heartbeat_receiver( context* context ) ;
    ~heartbeat_receiver( );
    void operator()();
  };
}

#endif
