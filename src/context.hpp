#ifndef __CONTEXT_HPP__
#define __CONTEXT_HPP__

#include "kibitz.hpp"


namespace kibitz {

    typedef std::vector< string > worker_types_t;

  class context {
    
    string worker_type_name_;
    string worker_id_;
    po::variables_map application_configuration_;
    void* zmq_context_;
    void* message_bus_socket_;
    bool signalled_;
    callback heartbeat_handler_;
    thread_group threads_;


    worker_types_t get_worker_types() const;
  public:
    context( const po::variables_map& application_configuration );
    ~context();
    void set_worker_type( const string& worker_type_name ) ;
    void set_worker_id( const string& worker_id );
    void register_heartbeat_handler( callback heartbeat_handler );
    void start() ;
    void stop() ;
    void terminate();
    void* zmq_context() ;
    void send_internal_message( const char* message ) ;
    const po::variables_map& get_config() const; 
  };

  typedef shared_ptr< context > context_ptr;


}


#endif
