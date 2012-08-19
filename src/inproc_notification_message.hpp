#ifndef inproc_notification_message_hpp
#define inproc_notification_message_hpp


#include "notification_message.hpp"


namespace kibitz {



  class inproc_notification_message : public notification_message {
    int notification_;
  public:
    inproc_notification_message( const ptree& json ) ;
    inproc_notification_message( int notification );
    virtual string to_json() const ;
    int get_notification() const { return notification_; }
  };

  typedef shared_ptr<inproc_notification_message> inproc_notification_ptr_t;
}

#endif

