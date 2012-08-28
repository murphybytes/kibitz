#ifndef worker_broadcast_message_hpp
#define worker_broadcast_message_hpp

#include "notification_message.hpp"

namespace kibitz {
  
  namespace notification {
    static const char* CREATE_BINDINGS = "create_bindings";
    static const char* INITIALIZE_JOB = "initialize_job";
  }

  class worker_broadcast_message : public notification_message {

    string notification_;
  public:
    

    worker_broadcast_message( const ptree& json ) ;
    worker_broadcast_message( const string& notification ) ;
    virtual ~worker_broadcast_message() {}
    virtual string to_json() const;
    const string& notification() const { return notification_; }
  };

  typedef shared_ptr<worker_broadcast_message> worker_broadcast_message_ptr_t;
}

#endif
