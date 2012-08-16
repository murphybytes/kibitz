#include "heartbeat_receiver.hpp"

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
    

    const char* multicast_binding = context_->get_config()["multicast-binding"].as<string>().c_str() ;
    
    int rc = zmq_connect( socket, multicast_binding );

    if( rc ) {
      LOG(ERROR) << "heartbeat receiver failed to connect to multicast " << multicast_binding ;
      return;
    }

    rc = zmq_setsockopt( socket, ZMQ_SUBSCRIBE, "", 0);

    if( rc ) {
      LOG(ERROR) << "Attempt to set socket options failed. " << __FILE__ << " " << __LINE__;
      return;
    }

    while( !shutdown() ) {
      DLOG(INFO) << "Waiting for heartbeat";
      string msg ;
      recv( socket, msg );
      DLOG(INFO) << "Received heartbeat" << msg;      
    }

    

    
  }

}
