#ifndef basic_collaboration_message_hpp
#define basic_collaboration_message_hpp

#include "collaboration_message.hpp"




namespace kibitz {
  class basic_collaboration_message : public collaboration_message {
    string worker_type_;
    string payload_;

  public:
    basic_collaboration_message( const string& worker_type, const string& payload ) 
      :collaboration_message( "generic" ),
       worker_type_( worker_type ),
       payload_(payload) {
    }

    basic_collaboration_message( const ptree& json ) 
      :collaboration_message( json ),
       worker_type_( json.get<string>( "worker_type" ) ),
       payload_( json.get<string>( "payload" ) ) {

    }

    virtual ~basic_collaboration_message() {}
    const string& worker_type() const { return worker_type_ ; }
    const string& payload() const { return payload_; } 
    virtual string to_json() const ;
    
  };

  typedef shared_ptr<basic_collaboration_message> basic_collaboration_message_ptr_t;
}

#endif
