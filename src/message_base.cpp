
#include "message_base.hpp"
#include "kibitz_util.hpp"


using kibitz::util::create_socket;
using kibitz::util::close_socket;
using kibitz::util::check_zmq;

namespace kibitz {


  message_base::message_base( context* context ) 
    :shutdown_(new bool(false)),
     context_( context ) {
  }

  message_base::~message_base() {
    //delete shutdown_;
  }

  bool message_base::shutdown() const {
    return *shutdown_;
  }

  void message_base::internal_command_handler( ) {
    DLOG(INFO) << "Created internal thread message handler";
    void* socket = NULL;
    try {
      socket = create_socket(  context_->zmq_context(), ZMQ_SUB);
      check_zmq( zmq_connect( socket, INPROC_COMMAND_BINDING ) );
      check_zmq( zmq_setsockopt( socket, ZMQ_SUBSCRIBE, "", 0) );
    

      while( true ) {
	string message;
	kibitz::util::recv( socket, message );
	DLOG(INFO) << "Message bus received " << message ;

	if( message == SHUTDOWN_MSG ) {
	  DLOG(INFO) << "setting shutdown flag";
	  *shutdown_ = true;
	  break;
	}
      }
    } catch( const util::queue_interrupt& ) {
      LOG(INFO) << "Caught interrupt" ;
    } catch( const std::exception& ex ) {
      LOG(ERROR) << "Exception terminated thread " << ex.what() ;
    }

    close_socket( socket );

  }

}

