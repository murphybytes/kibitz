#include <zmq.h>
#include <assert.h>
#include "kibitz_util.hpp"

namespace kibitz {
  namespace util {
    queue_interrupt::queue_interrupt( const string& msg )
      :runtime_error( msg ) {
    }

    queue_interrupt::~queue_interrupt() throw() {
    }

    void recv( void* socket, string& message ) {
      assert( message.empty() );
      zmq_msg_t msg;
      zmq_msg_init( &msg );
      int rc = zmq_recvmsg( socket, &msg, 0 );

      if( rc ) {
	int error = zmq_errno();
	if( EINTR == error ) {
	  throw queue_interrupt( "Received interrupt" );
	} else {
	  stringstream stm;
	  stm << "Error " << error << " at " << __FILE__ << " " << __LINE__;
	  throw runtime_error( stm.str() );
	}
      }

      int size = zmq_msg_size( &msg );
      const char* data = static_cast<const char*>( zmq_msg_data( &msg ) );
      message.append( data, size );
      zmq_msg_close( &msg );
    }
  }
}
