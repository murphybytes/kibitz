#include "heartbeat_sender.hpp"


namespace kibitz {

  heartbeat_sender::heartbeat_sender( context_ptr context )
    :message_base( context ) {
  }

  heartbeat_sender::~heartbeat_sender() {
  }
  
  void heartbeat_sender::operator()() {
    DLOG(INFO) << "entered heartbeat sender thread";
    while(!shutdown()) {
      sleep( 2 );
      DLOG(INFO) << "Heartbeat" ;
    }
    DLOG(INFO) << "Exiting heartbeat";
  }

}
