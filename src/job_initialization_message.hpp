#ifndef job_initialization_message_hpp
#define job_initialization_message_hpp

#include "notification_message.hpp"

namespace kibitz {
  namespace notification {
    static const char* JOB_INITIALIZATION = "job_initialization";
  }
  /**
   *  Message used to start jobs.  
   */
  class job_initialization_message : public notification_message {
    string worker_type_;
    int worker_id_;
  public:
    job_initialization_message( const ptree& json ) ;
    job_initialization_message( const string& worker_type, int worker_id  );
    virtual ~job_initialization_message( ) {}
    const string& worker_type() const { return worker_type_; }
    const int worker_id() const { return worker_id_ ; }

    virtual string to_json() const ;
  };

  typedef shared_ptr<job_initialization_message> job_initialization_message_ptr_t ;
}


#endif
