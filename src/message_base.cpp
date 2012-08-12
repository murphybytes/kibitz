
#include "message_base.hpp"


namespace kibitz {


  message_base::message_base( context_ptr context ) 
    :shutdown_(shared_ptr<bool>(new bool(false))),
     context_( context ),
     thread_( shared_ptr<boost::thread>(new boost::thread(boost::bind( &message_base::internal_command_handler, this ))) ) {

  }

  message_base::~message_base() {
  }

  bool message_base::shutdown() const {
    return *shutdown_;
  }

  void message_base::internal_command_handler( ) {
    DLOG(INFO) << "Created internal thread message handler";
    void* socket = zmq_socket( context_->zmq_context(), ZMQ_SUB);
    if( !socket ) {
      LOG(ERROR) << "Unable to create socket for internal message bus errno = " << errno ;
      return;
    }

    int rc = zmq_connect( socket, INPROC_COMMAND_BINDING ) ;
    
    if( rc ) {
      LOG(ERROR) << "Could not connect to " << INPROC_COMMAND_BINDING << " error " << errno ;
      return;
    }

    rc = zmq_setsockopt( socket, ZMQ_SUBSCRIBE, "", 0);

    if( rc ) {
      LOG(ERROR) << "setsockopt failed " << __FILE__ << " " << __LINE__ ;
    }
    

    while( true ) {

      string message;
      kibitz::recv( socket, message );
      DLOG(INFO) << "Message bus received " << message ;

      if( message == SHUTDOWN_MSG ) {
	DLOG(INFO) << "setting shutdown flag";
	*shutdown_ = true;
	break;
      }
    }

    zmq_close( socket );

  }

}

