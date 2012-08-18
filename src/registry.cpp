#include "registry.hpp"
#include <zmq.h>
#include "locator.hpp"
#include <sstream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
using boost::property_tree::ptree;

using namespace google;
using boost::dynamic_pointer_cast;

registry::registry( void* context, int port ) 
  :map_ptr_( new map_t() ),
   context_( context ),
   port_(port),
   inproc_pub_socket_(0),
   inproc_sub_socket_(0) {
  inproc_pub_socket_ = zmq_socket( context_, ZMQ_PUB );
  if( !inproc_pub_socket_ ) {
    throw runtime_error( "could not create socket" );
  }
  
  int rc = zmq_bind( inproc_pub_socket_, "inproc://x" );
  if( rc ) {
    throw runtime_error( "could not bind to inproc socket" );
  }

  inproc_sub_socket_ = zmq_socket( context_, ZMQ_SUB );
  if( !inproc_sub_socket_ ) {
    throw runtime_error( "could not creat inproc subscriber" );
  } 

  rc = zmq_setsockopt( inproc_sub_socket_, ZMQ_SUBSCRIBE, "", 0 );
  
  if( rc ) {
    throw runtime_error( "setsockoptions failed" );
  }

  rc = zmq_connect( inproc_sub_socket_, "inproc://x" );

  if( rc ) {
    throw runtime_error( "could not connect inproc socket" );
  }
  
}

registry::~registry() {
  if( inproc_pub_socket_ ) {
    zmq_close( inproc_pub_socket_ );
  }
}


void registry::push_message( const string& msg ) {
  DLOG(INFO) << "inproc message send " << msg;
  kibitz::util::send( inproc_pub_socket_, msg );
}


void registry::push_message( const kibitz::message& message ) {
  DLOG(INFO) << "sending message";
  kibitz::util::send( inproc_pub_socket_, message.to_json() );
}



void registry::operator()() {
  
  DLOG(INFO) << "starting registry thread";

  stringstream stm;
  stm << "tcp://*:" << port_;
  void* send_socket = zmq_socket( context_, ZMQ_PUB );
  if( !send_socket ) {
    LOG(ERROR) << "Socket creation failed " << zmq_errno() ;
    return;
  }

  int rc = zmq_bind( send_socket, stm.str().c_str() );

  if( rc ) {
    LOG(ERROR) << "Could not bind to " << stm.str() << " Error " << zmq_errno() ;
    return;
  }


  try {
    while( true ) {
      string message;
      kibitz::util::recv( inproc_sub_socket_, message ) ;
      kibitz::message_ptr message_ptr = kibitz::message_factory( message );
      assert( message_ptr->message_class() == "notification" );
      if( dynamic_pointer_cast<kibitz::notification_message>(message_ptr)->message_type() == "inproc" ) {
	int notification = dynamic_pointer_cast<kibitz::inproc_notification_message>(message_ptr)->get_notification() ;
	bool exit_thread = false;
	switch( notification ) {
	case kibitz::message::stop :
	  exit_thread = true;
	  break;
	default :
	  DLOG(INFO) << "Got a message we did not handle. notification = " << notification ;
	  break;
	}

	if( exit_thread ) {
	  break;
	}
      }

      if( dynamic_pointer_cast<kibitz::notification_message>(message_ptr)->message_type() == "heartbeat" ) {
	DLOG(INFO) << "Sender thread got heartbeat" ;
      }
    }
  } catch( const kibitz::util::queue_interrupt& ) {
    LOG(INFO) << "Shutting down sender thread";
  } catch( const std::exception& ex ) {
    LOG(ERROR) << "An exception killed sender thread " << __FILE__ << " " << __LINE__ ;
  }

  zmq_close( inproc_sub_socket_ );
  zmq_close( send_socket );
}
