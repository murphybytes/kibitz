#ifndef worker_map_hpp
#define worker_map_hpp


#include "kibitz.hpp"
#include "context.hpp"
#include "kibitz_util.hpp"
#include "worker_notification_message.hpp"
#include "heartbeat.hpp"
#include <map>

namespace kibitz {
  
  class worker_map {
    typedef int worker_id_t;
    typedef string worker_type_t;
    typedef std::map< worker_id_t, worker_notification_message_ptr_t > worker_by_id_t;
    typedef std::map< string, worker_by_id_t > worker_map_t;
    void* send_socket_;
    context* context_;
    worker_map( void* send_socket );
    void handle_worker_notification( message_ptr_t message, worker_map_t& workers ) ;
  public:
    worker_map(context* context) ;
    virtual ~worker_map();
    void operator()();
    void send_worker_notification_from_heartbeat( const string& json ) ;
    static shared_ptr<worker_map> get_worker_map( void* context );
    
  };
}

#endif
