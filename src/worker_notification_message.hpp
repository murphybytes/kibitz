#ifndef worker_notification_message_hpp
#define worker_notification_message_hpp

#include "heartbeat.hpp"


namespace kibitz {

  class worker_notification_message : public notification_message {
    string worker_type_;
    int32_t worker_id_;
    string host_name_;
    int16_t port_;


  public:
    worker_notification_message( const ptree& json ) ;
    worker_notification_message( const heartbeat& hb );
    worker_notification_message( const string& worker_type );
    virtual ~worker_notification_message() {}
    const string& worker_type() const ;
    const string& host_name() const;
    int32_t worker_id() const;
    int16_t port() const ;
    virtual string to_json() const;    

    
  };

}
#endif
