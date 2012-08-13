#include "heartbeat_sender.hpp"


namespace kibitz {

  heartbeat_sender::heartbeat_sender( context* context )
    :message_base( context ) {
  }

  heartbeat_sender::~heartbeat_sender() {
  }
  
  void heartbeat_sender::operator()() {
    DLOG(INFO) << "entered heartbeat sender thread";

    boost::thread message_bus_listener( boost::bind( &message_base::internal_command_handler, this ) );
    
    void* socket = zmq_socket(context_->zmq_context(), ZMQ_PUB );
    if( !socket ) {
      LOG(ERROR) << "Unable to create heartbeat sender socket";
      return;
    }

    const char* multicast_binding = context_->get_config()["multicast-binding"].as<string>().c_str();
    int rc = zmq_connect( socket, multicast_binding );

    if( rc ) {
      LOG(ERROR) << "Attempt to subscribe to multicast channel " << multicast_binding << " failed.";
      return;
    }

    while(!shutdown()) {
      sleep( 2 );
      DLOG(INFO) << "Heartbeat" ;
    }

    zmq_close( socket );
    DLOG(INFO) << "Exiting heartbeat";
  }

}
