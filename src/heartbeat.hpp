#ifndef __HEARTBEAT_HPP__
#define __HEARTBEAT_HPP__

// TODO: extend notification message

#include "kibitz.hpp"
#include "context.hpp"

namespace kibitz {

  class heartbeat {
    string worker_type_;
    int worker_id_;
    string host_name_;
    int pid_;
    int port_;
  public :
    heartbeat(context* ctx) ;
    ~heartbeat() ;
    string to_json() const ;
    
  };

}


#endif

