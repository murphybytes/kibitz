#include "kibitz.hpp"

namespace kibitz {
  context& get_context() {
    static context* p = NULL;
    if( !p ) {
      p = new context();
    }
    return *p;
  }

  context::context() {
    std::cout << "ctor" << std::endl;
  }

  void context::set_worker_type( const string& worker_type_name ) {
    worker_type_name_ = worker_type_name;
  }

  void context::set_worker_id( const string& worker_id ) {
    worker_id_ = worker_id ;
  }

  void context::register_heartbeat_handler( callback heartbeat_handler ) {
    heartbeat_handler_ = heartbeat_handler;
  }
}

