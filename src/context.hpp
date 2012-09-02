#ifndef __CONTEXT_HPP__
#define __CONTEXT_HPP__


#include "common.hpp"
#include "bus.hpp"

namespace kibitz {

    typedef std::vector< string > worker_types_t;

  class context {
    
    string worker_type_name_;
    string worker_id_;
    po::variables_map application_configuration_;
    void* zmq_context_;
    void* message_bus_socket_;
    bool signalled_;
    //    callback heartbeat_handler_;
    collaboration_callback inedge_message_handler_;
    initialization_callback initialization_handler_;
    thread_group threads_;
    boost::mutex mutex_;
    string current_job_id_;
    shared_ptr< pub > collaboration_publisher_ptr_;
    shared_ptr< pub > notification_publisher_ptr_;

  public:
    context( const po::variables_map& application_configuration );
    ~context();
    void set_worker_type( const string& worker_type_name ) ;
    void set_worker_id( const string& worker_id );
    
    const string& worker_type() const { return application_configuration_["worker-type"].as<string>(); }
    int worker_id() { return application_configuration_["worker-id"].as<int>() ; } 

    void register_initialization_notification_handler( initialization_callback initialization_handler );
    void register_inedge_message_handler( collaboration_callback inedge_message_handler ) ;
    collaboration_callback get_inedge_message_handler() ;
    initialization_callback get_initialization_notification_callback() ; 
    void start() ;
    void stop() ;
    void terminate();
    void* zmq_context() ;
    void send_internal_message( const char* message ) ;
    worker_types_t get_worker_types() const;
   
    const po::variables_map& get_config() const; 

    void send_out_message( const string& payload ) ;
    void send_notification_message( const string& payload );
    void set_job_id( const string& job_id ) ;
    void get_job_id( string& job_id );
  
  };

  typedef shared_ptr< context > context_ptr;


}


#endif
