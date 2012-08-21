
#ifndef __COLLABORATION_MESSAGE_HPP__
#define __COLLABORATION_MESSAGE_HPP__

#include  "message.hpp"

namespace kibitz {
  class collaboration_message : public message {
    const string collaboration_type_;
    
  public:
    collaboration_message( const string& collaboration_type, const string& version = "1.0" )
      :message( "collaboration", version ),
       collaboration_type_( collaboration_type ) {
    }
    virtual string to_json() const = 0;
    const string& collaboration_type() const { return collaboration_type_ ; }
  };

  typedef shared_ptr<collaboration_message> collaboration_message_ptr_t;
}

#endif



