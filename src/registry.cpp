#include "registry.hpp"
#include <zmq.h>
#include <sys/time.h>
#include "locator.hpp"
#include <sstream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
using boost::property_tree::ptree;

using namespace google;
using boost::dynamic_pointer_cast;

registry::registry( void* inproc_pub_socket, void* inproc_sub_socket,  void* publisher_socket ) 
  :publisher_socket_( publisher_socket ),
   inproc_pub_socket_(inproc_pub_socket),
   inproc_sub_socket_(inproc_sub_socket) {
   }

registry::~registry() {

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
  worker_map_t workers;
  timeval last_send = { 0, 0 };

  try {
    while( true ) {
      string message;
      kibitz::util::recv( inproc_sub_socket_, message ) ;
      DLOG(INFO) << "Registry got a message " << message ;
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
	kibitz::heartbeat hb = *dynamic_pointer_cast<kibitz::heartbeat>(message_ptr); 
	if( workers.find( hb.worker_type() ) == workers.end() ) {
	  worker_set_t workers_by_type;
	  workers_by_type.insert( hb );
	  workers[ hb.worker_type() ] = workers_by_type; 
	} else {
	  workers[ hb.worker_type() ].insert( hb );
	}
	
	if( one_second_elapsed( last_send ) ) {
	  DLOG(INFO) << "Sending locator information for " << workers.size() << " workers";
	  for( worker_map_t::iterator worker_type = workers.begin(); worker_type != workers.end(); ++worker_type ) {
	    DLOG(INFO) << "Sending Message for worker type " << worker_type->first ;
	    for( worker_set_t::iterator worker = worker_type->second.begin(); worker != worker_type->second.end(); ++worker ) {
	      DLOG(INFO) << "Sending worker type " << worker_type->first << " worker id " << worker->worker_id() ;
	      kibitz::util::send( publisher_socket_, worker->to_json() );	      
	    }
	  }

	}
	
      }
    }
  } catch( const kibitz::util::queue_interrupt& ) {
    LOG(INFO) << "Shutting down sender thread";
  } catch( const std::exception& ex ) {
    LOG(ERROR) << "An exception killed sender thread " << __FILE__ << " " << __LINE__ ;
  }

  
}

bool registry::one_second_elapsed( timeval& last_send ) {
  bool second_elapsed = false;
  const int microsecs_in_second = 1000000;
  timeval current_time;
  gettimeofday( &current_time, NULL );
  long curr_usec = (current_time.tv_sec * microsecs_in_second) + current_time.tv_usec;
  long last_usec = (last_send.tv_sec * microsecs_in_second) +  last_send.tv_usec;
  if( (curr_usec - last_usec) > microsecs_in_second ) {
    last_send = current_time;
    second_elapsed = true;
  }
  return second_elapsed;
  
}
