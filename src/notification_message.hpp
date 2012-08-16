#ifndef notification_message_hpp
#define notification_message_hpp


#include "message.hpp"


namespace kibitz {

  class notification_message : public message {
  public :
    virtual string to_json() const = 0;
  };

}


#endif


