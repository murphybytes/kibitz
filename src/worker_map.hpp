#ifndef worker_map_hpp
#define worker_map_hpp


#include "kibitz.hpp"
#include "context.hpp"
#include "kibitz_util.hpp"
#include "worker_notification_message.hpp"
#include "worker_query.hpp"
#include "worker_query_response.hpp"
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
    worker_infos_t process_query( const string& worker_type, const worker_map_t& workers ) const; 
  public:
    worker_map(context* context) ;
    virtual ~worker_map();
    void operator()();
    void send_worker_notification_from_heartbeat( const string& json ) ;
    /**
     * Retrieves connection information for inedge workers
     */
    worker_infos_t get_in_edge_workers( const string& worker_type ) const ;
    static shared_ptr<worker_map> get_worker_map( void* context );
    
  };

  typedef shared_ptr<worker_map> worker_map_ptr_t;
}

#endif
