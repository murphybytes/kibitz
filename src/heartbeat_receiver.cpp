#include "heartbeat_receiver.hpp"
#include "kibitz_util.hpp"

namespace kibitz {


  heartbeat_receiver::heartbeat_receiver( context* context )
    :message_base( context ) {
  }

  heartbeat_receiver::~heartbeat_receiver() {
  }

  void heartbeat_receiver::operator()() {
    DLOG(INFO) << "Entered heartbeat receiver thread.";
    
    boost::thread message_bus_listener( boost::bind( &message_base::internal_command_handler, this ));

    void* socket = zmq_socket( context_->zmq_context(), ZMQ_SUB );

    if( !socket ) {
      LOG(ERROR) << "Unable to create heartbeat receiver";
      return;
    }
    

    const char* binding = context_->get_config()["discovery-binding"].as<string>().c_str() ;
    LOG(INFO) << "Will subscribe to messages from locator on " << binding;
    int rc = zmq_connect( socket, binding );

    if( rc ) {
      LOG(ERROR) << "heartbeat receiver failed to connect to binding " << binding ;
      return;
    }

    rc = zmq_setsockopt( socket, ZMQ_SUBSCRIBE, "", 0);

    if( rc ) {
      LOG(ERROR) << "Attempt to set socket options failed. " ;
      return;
    }

    while( !shutdown() ) {
      DLOG(INFO) << "Waiting for heartbeat";
      string msg ;
      kibitz::util::recv( socket, msg );
      DLOG(INFO) << "Received heartbeat" << msg;      
    }

    

    
  }

}
