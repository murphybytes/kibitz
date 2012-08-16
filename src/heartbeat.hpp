#ifndef __HEARTBEAT_HPP__
#define __HEARTBEAT_HPP__


#include "kibitz.hpp"
#include "context.hpp"
#include "notification_message.hpp"

namespace kibitz {

  class heartbeat : public notification_message {
    string worker_type_;
    int worker_id_;
    string host_name_;
    int pid_;
    int port_;
    int ticks_;
  public :
    heartbeat(context* ctx) ;
    heartbeat( context* ctx, const ptree& json );
    virtual ~heartbeat() ;
    virtual string to_json() const ;
    
  };

}


#endif

