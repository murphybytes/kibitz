#include <zmq.h>
#include <assert.h>
#include <string.h>
#include "kibitz_util.hpp"

namespace kibitz {
  namespace util {
    queue_interrupt::queue_interrupt( const string& msg )
      :runtime_error( msg ) {
    }

    queue_interrupt::~queue_interrupt() throw() {
    }

    void close_socket( void* socket ) {
      if( socket ) {
	zmq_close( socket );
      }
    }

    void* create_socket( void* context, int socktype ) {
      void* result = zmq_socket( context, socktype ) ;
      if( !result ) {
	stringstream stm;
	stm << "zmq_socket failed with " << zmq_errno(); 
	throw std::runtime_error( stm.str() );
      }
      return result;
    }

    void check_zmq( int return_code ) {
      if( return_code ) {
	stringstream stm;
	stm << "zmq call failed with error code " << zmq_errno() ;
	throw std::runtime_error( stm.str() );
      }
    }

    void send( void* socket, const string& message ) {
      zmq_msg_t msg;
      zmq_msg_init_size( &msg, message.length() );
      memcpy( zmq_msg_data( &msg ), message.data(), message.length() );
      zmq_sendmsg( socket, &msg, 0 );
      zmq_msg_close( &msg );
    }

    void recv( void* socket, string& message ) {
      assert( message.empty() );
      zmq_msg_t msg;
      zmq_msg_init( &msg );
      int rc = zmq_recvmsg( socket, &msg, 0 );

      if( rc < 0 ) {
	int error = zmq_errno();
	if( EINTR == error ) {
	  throw queue_interrupt( "Received interrupt" );
	} else {
	  stringstream stm;
	  stm << "Error " << error ;
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
