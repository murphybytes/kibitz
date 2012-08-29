#include "context.hpp"
#include "heartbeat_sender.hpp"
#include "heartbeat_receiver.hpp"
#include "kibitz_util.hpp"
#include "worker_map.hpp"
#include <yaml-cpp/yaml.h>
#include "in_edge_manager.hpp"
#include "basic_collaboration_message.hpp"

namespace kibitz {


  context::context( const po::variables_map& application_configuration )
    :application_configuration_(application_configuration),
     zmq_context_(NULL),
     signalled_(false),
     inedge_message_handler_(NULL),
     initialization_handler_(NULL) {

    DLOG(INFO) << "ctor for context entered" ;
    zmq_context_ = zmq_init( application_configuration["context-threads"].as<int>() );
    DLOG(INFO) << "zmq initialized";
    message_bus_socket_ = util::create_socket( zmq_context_, ZMQ_PUB );
    util::check_zmq( zmq_bind( message_bus_socket_, INPROC_COMMAND_BINDING ) );
    stringstream stm1, stm2;
    stm1 << "tcp://*:" << application_configuration["publish-port"].as<int>();
    stm2 << "tcp://*:" << application_configuration["notification-port"].as<int>();
    
    collaboration_publisher_ptr_ = shared_ptr<pub>( new pub( zmq_context_, stm1.str().c_str()   ) );

    notification_publisher_ptr_ = shared_ptr<pub>( new pub( zmq_context_, stm2.str().c_str() ) );
  }

  context::~context() {
  }

  void context::send_out_message( const string& payload )  {
    string worker_type = application_configuration_["worker-type"].as<string>();
    string job_id;
    get_job_id( job_id );
    string json;
    if( job_id.empty() ) {
      basic_collaboration_message msg( worker_type, payload );
      json = msg.to_json();
    } else {
      basic_collaboration_message msg( worker_type, job_id, payload );
      json = msg.to_json();
    }
    
    collaboration_publisher_ptr_->send( json );

  }

  void context::send_notification_message( const string& payload ) {
    notification_publisher_ptr_->send( payload );
  }

  void context::register_initialization_notification_handler( initialization_callback initialization_handler ) {
    initialization_handler_ = initialization_handler;
  }

  initialization_callback context::get_initialization_notification_callback() {
    return initialization_handler_;
  }

  void context::register_inedge_message_handler( collaboration_callback inedge_message_handler ) {
    inedge_message_handler_ = inedge_message_handler;
  }

  collaboration_callback context::get_inedge_message_handler() {
    return inedge_message_handler_;
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

  worker_types_t context::get_worker_types() const {
    worker_types_t worker_types;
    if( application_configuration_.count( "configuration-file" ) ) {
      std::fstream stream( application_configuration_["configuration-file"].as<string>().c_str() );
      YAML::Parser parser( stream );
      YAML::Node doc;
      parser.GetNextDocument( doc );
      YAML::Iterator it = doc.begin();
      it.second() >> worker_types;
    }
    return worker_types;
  }

  
  

  void context::start() {
    heartbeat_sender hb_sender( this );
    heartbeat_receiver hb_receiver( this );
    worker_map wmap( this );
    kibitz::in_edge_manager in_edge_manager( *this ); 

    threads_.create_thread( wmap );
    threads_.create_thread(hb_sender);
    threads_.create_thread(hb_receiver);
    threads_.create_thread(in_edge_manager);

    
    threads_.join_all();

  }

  

  void context::stop() {
  }

  void context::terminate() {
    DLOG(INFO) << "context.terminate shutting down application" ;
    collaboration_publisher_ptr_->close();
    notification_publisher_ptr_->close();

    util::close_socket( message_bus_socket_ );

    if( zmq_context_ ) {
      zmq_term( zmq_context_ );
    }
  }

  void* context::zmq_context() {
    return zmq_context_; 
  }


  void context::set_job_id( const string& job_id ) {
    boost::mutex::scoped_lock lock( mutex_ ) ;
    current_job_id_ = job_id;
  }

  void context::get_job_id( string& job_id ) {
    boost::mutex::scoped_lock lock( mutex_ );
    job_id = current_job_id_;
  }
 
}

