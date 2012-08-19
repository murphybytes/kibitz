#include "heartbeat.hpp"
#include "heartbeat_sender.hpp"
#include "kibitz_util.hpp"

using kibitz::util::create_socket;
using kibitz::util::check_zmq;
using kibitz::util::close_socket;

namespace kibitz {

  heartbeat_sender::heartbeat_sender( context* context )
    :message_base( context ) {
  }

  heartbeat_sender::~heartbeat_sender() {
  }
  
  void heartbeat_sender::operator()() {
    DLOG(INFO) << "entered heartbeat sender thread";

    
    void* socket = NULL; 

    try {
      socket = create_socket( context_->zmq_context(), ZMQ_PUSH );
      const char* binding = context_->get_config()["heartbeat-binding"].as<string>().c_str();
      check_zmq( zmq_connect( socket, binding ) );

      heartbeat beater( context_->get_config() );
      boost::condition_variable condition;
      boost::mutex mutex;
      boost::unique_lock<boost::mutex> lock( mutex );
      time_duration duration = millisec( context_->get_config()["heartbeat-frequency"].as<int>() );

      while(true) {
	condition.timed_wait( lock, duration ); 
	beater.increment_tick_count() ;
	DLOG(INFO) << "Generated heartbeat"; 
	kibitz::util::send( socket, beater.to_json() );
      }
    } catch( const std::exception& ex ) {
      LOG(ERROR) << "Something went horribly awry or maybe we just caught an interrupt. Details " << ex.what() ;
    }

    close_socket( socket );
    DLOG(INFO) << "Exiting heartbeat";
  }

}
