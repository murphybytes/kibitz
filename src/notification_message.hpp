#ifndef notification_message_hpp
#define notification_message_hpp


#include "message.hpp"


namespace kibitz {

  class notification_message : public message {
    const string message_type_;
  protected:
    notification_message(const string& message_type) 
      :message("notification"),
       message_type_(message_type) {}
  public :
    virtual string to_json() const = 0;
    const string& message_type() const { return message_type_ ; }
  };

}


#endif


