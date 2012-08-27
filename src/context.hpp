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
    callback inedge_message_handler_;
    thread_group threads_;
    boost::mutex mutex_;


  public:
    context( const po::variables_map& application_configuration );
    ~context();
    void set_worker_type( const string& worker_type_name ) ;
    void set_worker_id( const string& worker_id );
    void register_heartbeat_handler( callback heartbeat_handler );
    void register_inedge_message_handler( callback inedge_message_handler ) ;
    callback get_inedge_message_handler() ; 
    void start() ;
    void stop() ;
    void terminate();
    void* zmq_context() ;
    void send_internal_message( const char* message ) ;
    worker_types_t get_worker_types() const;
    /**
     * Adds a thread to message processing threads, primarily used 
     * to switch out inedge monitoring threads
     */
    template< typename F >
    boost::thread* swap_thread( F new_thread_fnc, boost::thread* old_thread = NULL) {
      boost::mutex::scoped_lock( mutex_ );
      if( old_thread ) { 
	threads_.remove_thread( old_thread );
      }
      return threads_.create_thread( new_thread_fnc );
    }
    const po::variables_map& get_config() const; 
  };

  typedef shared_ptr< context > context_ptr;


}


#endif
