
#include "message_base.hpp"


namespace kibitz {


  message_base::message_base( context_ptr context ) 
    :context_( context ),
     thread_( shared_ptr<boost::thread>(new boost::thread(boost::bind( &message_base::internal_command_handler, this ))) ) {

  }

  message_base::~message_base() {
  }

  void message_base::internal_command_handler( ) {
    DLOG(INFO) << "Created internal thread message handler";
    void* sock = zmq_socket( context_->zmq_context(), ZMQ_SUB);
    

  }

}

