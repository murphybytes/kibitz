#include "heartbeat_receiver.hpp"
#include "kibitz_util.hpp"

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
      socket = create_socket( context_->zmq_context(), ZMQ_SUB );
      const char* binding = context_->get_config()["discovery-binding"].as<string>().c_str() ;
      LOG(INFO) << "Will subscribe to messages from locator on " << binding;
      check_zmq( zmq_connect( socket, binding ) );
      check_zmq(zmq_setsockopt( socket, ZMQ_SUBSCRIBE, "", 0));

      while( true ) {
	DLOG(INFO) << "Waiting for heartbeat";
	string msg ;
	// TODO if we dont receive a heartbeat after a certain amount of time
	// try to connect to alternative locator
	kibitz::util::recv( socket, msg );
	DLOG(INFO) << "Received heartbeat" << msg;      
      }
    } catch( const util::queue_interrupt& ) {
      LOG(INFO) << "Received interrupt shutting down heartbeat receiver thread";
    } catch( const std::exception& ex ) {
      LOG(ERROR) << "Exception caused thread to terminate " << ex.what() ;
    }

    

    
  }

}
