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
    while(!shutdown()) {
      sleep( 2 );
      DLOG(INFO) << "Heartbeat" ;
    }
    DLOG(INFO) << "Exiting heartbeat";
  }

}
