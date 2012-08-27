#ifndef notification_message_hpp
#define notification_message_hpp


#include "message.hpp"


namespace kibitz {

  class notification_message : public message {
    const string notification_type_;
  protected:
    notification_message(const string& notification_type, const string& version = "1.0") 
      :message("notification", version ),
       notification_type_(notification_type) {}
    void populate_header( ptree& tree ) const {
      message::populate_header( tree );
      tree.put( "notification_type", notification_type_ );
    }
  public :
    virtual ~notification_message() {}
    virtual string to_json() const = 0;
    const string& notification_type() const { return notification_type_ ; }
  };

  typedef shared_ptr<notification_message> notification_message_ptr_t;

}


#endif


