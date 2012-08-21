#include "worker_map.hpp"
#include "inproc_notification_message.hpp"

namespace kibitz {


  worker_map::worker_map(context* context) 
    :send_socket_(NULL),
     context_(context)  {
    //send_socket_ = util::create_socket( context_->zmq_context(), ZMQ_REQ );
    //util::check_zmq( zmq_connect( send_socket_, WORKER_MANAGER_BINDING )); 
  }

  worker_map::~worker_map() {
    util::close_socket( send_socket_ );
  }

  worker_map::worker_map( void* send_socket ) 
    :send_socket_(send_socket),
     context_(NULL) {
  }

  shared_ptr<worker_map> worker_map::get_worker_map( void* context ) {
    void* socket = NULL;
    socket = util::create_socket( context, ZMQ_REQ );
    util::check_zmq( zmq_connect( socket, WORKER_MANAGER_BINDING ));
    return shared_ptr<worker_map>( new worker_map( socket ) );
  }

  void worker_map::send_worker_notification_from_heartbeat( const string& json ) {
    DLOG(INFO) << "WORKER NOTIFY " << json;
    heartbeat_ptr_t heartbeat_ptr = dynamic_pointer_cast<heartbeat>( message_factory( json ) ) ;
    worker_notification_message message( heartbeat_ptr );
    util::send( send_socket_, message.to_json() );
    string response;
    util::recv( send_socket_,response );
    inproc_notification_ptr_t notification_ptr = dynamic_pointer_cast<inproc_notification_message>(message_factory( response ));
   
    if( notification_ptr->get_notification() != message::ok ) {
      throw std::runtime_error( "Attempt to send worker notification failed" );
    }
       
  }

  void worker_map::handle_worker_notification( message_ptr_t message, worker_map_t& workers )  {
    DLOG(INFO) << "handling worker notification" ;
    worker_notification_message_ptr_t worker = dynamic_pointer_cast<worker_notification_message>(message);
    // TODO timestamp when worker was placed into this map 
    // so we can deal with dead workers that don't heartbeat
    workers[worker->worker_type()][worker->worker_id()] = worker;
  }

  void worker_map::operator()() {
    DLOG(INFO) << "Worker map manager thread started";
    worker_map_t worker_map;
    void* socket = NULL;
    try {
      socket = util::create_socket( context_->zmq_context(), ZMQ_REP );
      util::check_zmq( zmq_bind( socket, WORKER_MANAGER_BINDING ) );
      while( true ) {
	string json ;
	util::recv( socket, json );
	DLOG(INFO) << "worker map manager got " << json;
	message_ptr_t message = message_factory( json );
	assert( message->message_type() == "notification" );
	if( dynamic_pointer_cast<notification_message>(message)->notification_type() == "worker_notification" ){
	  handle_worker_notification( message, worker_map );
	  inproc_notification_message response( message::ok );
	  util::send( socket, response.to_json() );
	}
	
      }
    } catch( const kibitz::util::queue_interrupt& ) {
      LOG(INFO) << "Worker map manager got interrupt shutting down";
    } catch( const std::exception& ex ) {
      LOG(ERROR) << "Exception: " << ex.what() ;
    }

    util::close_socket( socket );
  }
}
