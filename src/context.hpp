#ifndef __CONTEXT_HPP__
#define __CONTEXT_HPP__

#include "kibitz.hpp"


namespace kibitz {


  class context {
    
    string worker_type_name_;
    string worker_id_;
    callback heartbeat_handler_;

  public:
    context( const variables_map& application_configuration );
    void set_worker_type( const string& worker_type_name ) ;
    void set_worker_id( const string& worker_id );
    void register_heartbeat_handler( callback heartbeat_handler );
    
  };


}


#endif
