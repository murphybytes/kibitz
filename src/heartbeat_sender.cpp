#include "heartbeat_sender.hpp"


namespace kibitz {

  heartbeat_sender::heartbeat_sender( context_ptr context )
    :message_base( context ) {
  }

  heartbeat_sender::~heartbeat_sender() {
  }
  
  void heartbeat_sender::operator()() {


  }

}
