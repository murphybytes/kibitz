#ifndef __KIBITZ_HPP__
#define __KIBITZ_HPP__

#include <iostream>
#include <string>

using std::string;

namespace kibitz {

  typedef void ( *callback )( const string&  );

  class context {
    friend context& get_context();
    
    string worker_type_name_;
    string worker_id_;
    callback heartbeat_handler_;

    context();
  public:
    void set_worker_type( const string& worker_type_name ) ;
    void set_worker_id( const string& worker_id );
    void register_heartbeat_handler( callback heartbeat_handler );
    
  };

  context& get_context() ; 

}

#endif
