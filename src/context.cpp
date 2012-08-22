#include "context.hpp"
#include "heartbeat_sender.hpp"
#include "heartbeat_receiver.hpp"
#include "kibitz_util.hpp"
#include "worker_map.hpp"

namespace kibitz {


  context::context( const po::variables_map& application_configuration )
    :application_configuration_(application_configuration),
     zmq_context_(NULL),
     signalled_(false)  {
    DLOG(INFO) << "ctor for context entered" ;
    zmq_context_ = zmq_init( application_configuration["context-threads"].as<int>() );
    DLOG(INFO) << "zmq initialized";
    message_bus_socket_ = zmq_socket( zmq_context_, ZMQ_PUB );
    if( !message_bus_socket_ ) {
      throw std::runtime_error( "Failed to create message bus socket."); 
    }
    int rc = zmq_bind( message_bus_socket_, INPROC_COMMAND_BINDING ) ;
    if( rc ) {
      throw std::runtime_error( "Failed to create binding for internal message bus" );
    }
  }

  context::~context() {
  }

  const po::variables_map& context::get_config() const {
    return application_configuration_;
  } 

  void context::send_internal_message( const char* message ) {
    string s( message );
    DLOG(INFO) << "sending internal message -> " << message;
    kibitz::util::send( message_bus_socket_, s ); 
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
    heartbeat_sender hb_sender( this );
    heartbeat_receiver hb_receiver( this );
    worker_map wmap( this );
    //   edges_t in_edges = get_in_edges() ;
    //edges_t out_edges = get_out_edges();
    //out_edge_manager out_edges( this );
    threads_.create_thread( wmap );
    threads_.create_thread(hb_sender);
    threads_.create_thread(hb_receiver);
    /*
    for( workers::iterator worker = in_edges.begin(); worker != in_edges.end() ; ++worker ) {
      threads_.create( in_edge( this, worker ) );
    }

    for( workers::iterator worker = out_edges.begin(); worker != out_edges.end() ; ++worker ) {
      threads_.create( out_edge( this, worker );
    }
    */

          
    
    threads_.join_all();

  }

  void context::stop() {
  }

  void context::terminate() {
    DLOG(INFO) << "context.terminate shutting down application" ;

    if( message_bus_socket_ ) {
      zmq_close( message_bus_socket_ );
    }

    if( zmq_context_ ) {
      zmq_term( zmq_context_ );
    }
  }

  void* context::zmq_context() {
    return zmq_context_; 
  }

 
}

