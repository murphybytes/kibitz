#ifndef notification_message_hpp
#define notification_message_hpp


#include "message.hpp"


namespace kibitz {

  class notification_message : public message {
    const string notification_type_;
    const string version_;
  protected:
    notification_message(const string& notification_type, const string& version = "1.0") 
      :message("notification"),
       version_( version ),
       notification_type_(notification_type) {}
    void populate_header( ptree& tree ) const {
      message::populate_header( tree );
      tree.put( "notification_type", notification_type_ );
      tree.put( "version", version_ );
    }
  public :
    virtual string to_json() const = 0;
    const string& notification_type() const { return notification_type_ ; }
  };

}


#endif


