#include "context.hpp"
#include "heartbeat_sender.hpp"

namespace kibitz {


  context::context( const po::variables_map& application_configuration )
    :application_configuration_(application_configuration),
     zmq_context_(NULL),
     signalled_(false)  {
    DLOG(INFO) << "ctor for context entered" ;
    zmq_context_ = zmq_init( application_configuration["context-threads"].as<int>() );
    DLOG(INFO) << "zmq initialized";
  }

  context::~context() {
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

  void context::start() {
    heartbeat_sender hb_sender( context_ptr(this) );
    threads_.create_thread(hb_sender);
    threads_.join_all();

  }

  void context::stop() {
  }

  void context::terminate() {
    DLOG(INFO) << "context.terminate shutting down application" ;
    if( zmq_context_ ) {
      zmq_term( zmq_context_ );
    }
  }

  void* context::zmq_context() {
    return zmq_context_; 
  }


}

