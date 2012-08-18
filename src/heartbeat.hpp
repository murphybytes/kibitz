#ifndef __HEARTBEAT_HPP__
#define __HEARTBEAT_HPP__



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
    heartbeat(const boost::program_options::variables_map& config) ;
    heartbeat( const ptree& json );
    virtual ~heartbeat() ;
    virtual string to_json() const ;
    bool operator<(const heartbeat& comp ) const ;
    const string& worker_type() const ;
    const int& worker_id() const ;
    void increment_tick_count();
    //virtual message_type_t message_type() const { return heartbeat; }
  };

}


#endif

