#include "heartbeat.hpp"
#include "heartbeat_sender.hpp"
#include "kibitz_util.hpp"


namespace kibitz {

  heartbeat_sender::heartbeat_sender( context* context )
    :message_base( context ) {
  }

  heartbeat_sender::~heartbeat_sender() {
  }
  
  void heartbeat_sender::operator()() {
    DLOG(INFO) << "entered heartbeat sender thread";

    boost::thread message_bus_listener( boost::bind( &message_base::internal_command_handler, this ) );
    
    void* socket = zmq_socket(context_->zmq_context(), ZMQ_PUSH );
    if( !socket ) {
      LOG(ERROR) << "Unable to create heartbeat sender socket";
      return;
    }

    const char* binding = context_->get_config()["heartbeat-binding"].as<string>().c_str();
    int rc = zmq_connect( socket, binding );

    if( rc ) {
      LOG(ERROR) << "Attempt to subscribe to multicast channel " << binding << " failed.";
      return;
    }

    heartbeat beater( context_->get_config() );
    boost::condition_variable condition;
    boost::mutex mutex;
    boost::unique_lock<boost::mutex> lock( mutex );
    time_duration duration = millisec( 1000 );

    while(!shutdown()) {
      condition.timed_wait( lock, duration ); 
      // TODO: heartbeat freq configurable
      //sleep( 5 );
      beater.increment_tick_count() ;
      DLOG(INFO) << "Generated heartbeat"; 
      send( socket, beater.to_json() );
    }

    zmq_close( socket );
    DLOG(INFO) << "Exiting heartbeat";
  }

}
