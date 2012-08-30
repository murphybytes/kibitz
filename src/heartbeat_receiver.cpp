#include "heartbeat_receiver.hpp"
#include "kibitz_util.hpp"
#include "worker_map.hpp"
#include "heartbeat.hpp"
#include "worker_broadcast_message.hpp"
#include "job_initialization_message.hpp"
#include "bus.hpp"
#include <signal.h>
using kibitz::util::create_socket;
using kibitz::util::check_zmq;

namespace kibitz {


  heartbeat_receiver::heartbeat_receiver( context* context )
    :message_base( context ) {
  }

  heartbeat_receiver::~heartbeat_receiver() {
  }

  void heartbeat_receiver::operator()() {
    DLOG(INFO) << "Entered heartbeat receiver thread.";
    
    boost::thread message_bus_listener( boost::bind( &message_base::internal_command_handler, this ));

    void* socket = NULL;

    try {
      pub broadcast_publisher( context_->zmq_context(), HEARTBEAT_RECEIVER_BROADCASTS );
      socket = create_socket( context_->zmq_context(), ZMQ_SUB );
      const char* binding = context_->get_config()["discovery-binding"].as<string>().c_str() ;
      LOG(INFO) << "Will subscribe to messages from locator on " << binding;
      check_zmq( zmq_connect( socket, binding ) );
      check_zmq(zmq_setsockopt( socket, ZMQ_SUBSCRIBE, "", 0));
      shared_ptr<worker_map> worker_map_ptr = worker_map::get_worker_map( context_->zmq_context() );

      while( true ) {
	DLOG(INFO) << "Waiting for heartbeat";
	string json ;
	// TODO if we dont receive a heartbeat after a certain amount of time
	// try to connect to alternative locator
	kibitz::util::recv( socket, json );
	DLOG(INFO) << "Received message" << json;
	notification_message_ptr_t message_ptr = dynamic_pointer_cast<notification_message>( message_factory( json ) );
	if( message_ptr != NULL ) {
	  string notification_type = message_ptr->notification_type();
	  if( notification_type == "heartbeat" ) {
	    worker_map_ptr->send_worker_notification_from_heartbeat( json );
	  } else if( notification_type == "worker_broadcast"  ) {
	    worker_broadcast_message_ptr_t wb = dynamic_pointer_cast<worker_broadcast_message>( message_ptr ) ;
	    if( wb->notification() == "shutdown" ) {
	      exit( 0 ) ;
	    } else {
	      broadcast_publisher.send( message_ptr->to_json()  );
	    }
	  } else if( notification_type == notification::JOB_INITIALIZATION ) {
	    broadcast_publisher.send( message_ptr->to_json() );
	  } else {	
	    LOG(ERROR) << "We got a message we don't know how to handle - " << json;
	  }
	} else {
	  LOG(ERROR) << "Unknown message type. Raw message = " << json;
	}
	
      }
    } catch( const util::queue_interrupt& ) {
      LOG(INFO) << "Received interrupt shutting down heartbeat receiver thread";
    } catch( const std::exception& ex ) {
      LOG(ERROR) << "Exception caused thread to terminate " << ex.what() ;
    }

    util::close_socket( socket );

    
  }

}
