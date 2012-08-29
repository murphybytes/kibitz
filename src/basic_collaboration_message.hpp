#ifndef basic_collaboration_message_hpp
#define basic_collaboration_message_hpp

#include "collaboration_message.hpp"

namespace kibitz {
  class basic_collaboration_message : public collaboration_message {
    string payload_;
  public:
    basic_collaboration_message( const string& worker_type, const string& payload ) 
      :collaboration_message( "generic", worker_type, "1.0" ),
       payload_(payload) {
    }
    basic_collaboration_message( const string& worker_type, const string& job_id, const string& payload )
      :collaboration_message( "generic", worker_type, job_id, "1.0" ),
       payload_(payload) {
    }

    virtual ~basic_collaboration_message() {}
    virtual string to_json() const ;
    const string& payload() const { return payload_; }
  };

  typedef shared_ptr<basic_collaboration_message> basic_collaboration_message_ptr_t;
}

#endif
