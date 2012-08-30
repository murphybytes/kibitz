#ifndef basic_collaboration_message_hpp
#define basic_collaboration_message_hpp



#include "collaboration_message.hpp"




namespace kibitz {
  class basic_collaboration_message : public collaboration_message {
    string worker_type_;
    string payload_;

  public:
    basic_collaboration_message( const string& worker_type, const string& payload ) ;

    basic_collaboration_message( const ptree& json ) ;

    virtual ~basic_collaboration_message() {}
    const string& worker_type() const { return worker_type_ ; }
    const string& payload() const { return payload_; } 
    virtual string to_json() const ;
    
  };

  typedef shared_ptr<basic_collaboration_message> basic_collaboration_message_ptr_t;
}

#endif
