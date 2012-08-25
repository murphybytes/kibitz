#include "worker_map.hpp"
#include "inproc_notification_message.hpp"

namespace kibitz {


  worker_map::worker_map(context* context) 
    :send_socket_(NULL),
     context_(context)  {
    //send_socket_ = util::create_socket( context_->zmq_context(), ZMQ_REQ );
    //util::check_zmq( zmq_connect( send_socket_, WORKER_MANAGER_BINDING )); 
  }

  worker_map::~worker_map() {
    util::close_socket( send_socket_ );
  }

  worker_map::worker_map( void* send_socket ) 
    :send_socket_(send_socket),
     context_(NULL) {
  }

  shared_ptr<worker_map> worker_map::get_worker_map( void* context ) {
    void* socket = NULL;
    socket = util::create_socket( context, ZMQ_REQ );
    util::check_zmq( zmq_connect( socket, WORKER_MANAGER_BINDING ));
    return shared_ptr<worker_map>( new worker_map( socket ) );
  }

  worker_infos_t worker_map::get_in_edge_workers(const string& worker_type ) const {
    worker_query query( worker_type );
    util::send( send_socket_, query.to_json() );
    string response_json;
    util::recv( send_socket_, response_json );
    worker_query_response_ptr_t worker_query_response_ptr = dynamic_pointer_cast<worker_query_response>( message_factory( response_json ) ) ;
    return worker_query_response_ptr->get_workers() ;
  }

  void worker_map::send_worker_notification_from_heartbeat( const string& json ) {
    DLOG(INFO) << "WORKER NOTIFY " << json;
    heartbeat_ptr_t heartbeat_ptr = dynamic_pointer_cast<heartbeat>( message_factory( json ) ) ;
    worker_notification_message message( heartbeat_ptr );
    util::send( send_socket_, message.to_json() );
    string response;
    util::recv( send_socket_,response );
    inproc_notification_ptr_t notification_ptr = dynamic_pointer_cast<inproc_notification_message>(message_factory( response ));
   
    if( notification_ptr->get_notification() != message::ok ) {
      throw std::runtime_error( "Attempt to send worker notification failed" );
    }
       
  }

  void worker_map::handle_worker_notification( message_ptr_t message, worker_map_t& workers )  {
    DLOG(INFO) << "handling worker notification" ;
    worker_notification_message_ptr_t worker = dynamic_pointer_cast<worker_notification_message>(message);
    // TODO timestamp when worker was placed into this map 
    // so we can deal with dead workers that don't heartbeat
    workers[worker->worker_type()][worker->worker_id()] = worker;
  }

  worker_infos_t worker_map::process_query( const string& worker_type, const worker_map_t& workers ) const {
    worker_infos_t response;
    worker_map_t::const_iterator hits = workers.find( worker_type ) ;
    if( hits != workers.end() ) {
      typedef const std::pair< worker_id_t, worker_notification_message_ptr_t >&  pair_t;
      BOOST_FOREACH( pair_t hit, hits->second ) {
	worker_info worker ;
	worker.worker_type = hit.second->worker_type();
	worker.worker_id = hit.second->worker_id();
	worker.port = hit.second->port();
	worker.host = hit.second->host_name() ;
	response.push_back( worker );	
      }
    }
    return response;
    
  }

  void worker_map::operator()() {
    DLOG(INFO) << "Worker map manager thread started";
    worker_map_t worker_map;
    void* socket = NULL;
    try {
      socket = util::create_socket( context_->zmq_context(), ZMQ_REP );
      util::check_zmq( zmq_bind( socket, WORKER_MANAGER_BINDING ) );
      while( true ) {
	string json ;
	util::recv( socket, json );
	DLOG(INFO) << "worker map manager got " << json;
	message_ptr_t message = message_factory( json );
	assert( message->message_type() == "notification" );
	string notification_type = dynamic_pointer_cast<notification_message>(message)->notification_type(); 
	if( notification_type == "worker_notification" ){
	  handle_worker_notification( message, worker_map );
	  inproc_notification_message response( message::ok );
	  util::send( socket, response.to_json() );
	}

	if( notification_type == "worker_query" ) {
	  worker_query_ptr_t query_ptr = dynamic_pointer_cast<worker_query>( message );
	  //worker_infos_t worker_infos;

	  worker_map_t::iterator hits = worker_map.find( query_ptr->worker_type() ) ;
	  worker_infos_t worker_infos = process_query( query_ptr->worker_type(), worker_map );
	  worker_query_response wqr;
	  wqr.set_workers( worker_infos );
	  string wqr_response = wqr.to_json();
	  DLOG(INFO) << "Sending worker query response " << wqr_response;
	  util::send( socket, wqr_response );

	}
	
      }
    } catch( const kibitz::util::queue_interrupt& ) {
      LOG(INFO) << "Worker map manager got interrupt shutting down";
    } catch( const std::exception& ex ) {
      LOG(ERROR) << "Exception: " << ex.what() ;
    }

    util::close_socket( socket );
  }
}
