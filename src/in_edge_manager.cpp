#include "in_edge_manager.hpp"
#include "bus.hpp"
#include "kibitz_util.hpp"
#include "context.hpp"
#include "worker_broadcast_message.hpp"
#include "basic_collaboration_message.hpp"
#include "job_initialization_message.hpp"
#include "worker_map.hpp"

namespace kibitz {
  in_edge_manager::in_edge_manager( context& ctx  ) 
    :context_( ctx ),
     worker_type_( ctx.get_config()["worker-type"].as<string>() ),
     worker_id_( ctx.get_config()["worker-id"].as<int>() ) {
  }

  in_edge_manager::~in_edge_manager()   {
  }

  void in_edge_manager::create_bindings( zmq_pollitem_t** pollitems, int& count_items, int& size_items ) {
    release_bindings( *pollitems, count_items );
    worker_infos_t all_infos; 
    worker_map_ptr_t worker_map_ptr = worker_map::get_worker_map( context_.zmq_context() );
    worker_types_t worker_types = context_.get_worker_types();
    BOOST_FOREACH( const string& worker_type, worker_types ) {
      DLOG(INFO) << "Resolving workers for " << worker_type ;
      worker_infos_t worker_infos = worker_map_ptr->get_in_edge_workers( worker_type );
      DLOG(INFO) << "Got " << worker_infos.size() << " hits for " << worker_type;
      all_infos.insert( all_infos.end(), worker_infos.begin(), worker_infos.end() );
    }

    DLOG(INFO) << "Total in edges = " << all_infos.size() ;
    if( size_items < (all_infos.size() + 1) ) {
      size_items = all_infos.size() + 1;
      *pollitems = (zmq_pollitem_t*)realloc( *pollitems, size_items * sizeof(zmq_pollitem_t) );
    }

    // create bindings for all in edges, in edges
    // start at index 1, index 0 is broadcast notifications
    count_items = all_infos.size() + 1;
    int current = 1;
    BOOST_FOREACH( const worker_info& info, all_infos ) {
      create_binding( info, (*pollitems)[current] ); 
      current++;
    }

  }

  void in_edge_manager::create_binding( const worker_info& info, zmq_pollitem_t& pollitem ) {

    stringstream stm;
    stm << "tcp://" << info.host << ":" << info.port;
    pollitem.socket = util::create_socket( context_.zmq_context(), ZMQ_SUB );
    pollitem.fd = 0;
    pollitem.events = ZMQ_POLLIN;
    pollitem.revents = 0;
    util::check_zmq( zmq_connect( pollitem.socket, stm.str().c_str() ) );
    util::check_zmq( zmq_setsockopt( pollitem.socket, ZMQ_SUBSCRIBE, "", 0 ) );
    
  }

  void in_edge_manager::release_bindings( zmq_pollitem_t* pollitems, int count_items ) {
    // don't close item zero socket, its managed by broadcast_subscriber
    for( int item = 1; item < count_items; ++item ) {
      util::close_socket( pollitems[item].socket );
    }
  }

  void in_edge_manager::handle_notification_message( zmq_pollitem_t** pollitems, int& count_items, int& size_items ) {
    zmq_pollitem_t* items = *pollitems;
  
    if( (items[0].revents | ZMQ_POLLIN) == ZMQ_POLLIN ) {
      string json;
      util::recv( items[0].socket, json );
      DLOG(INFO) << "in edge manger got " << json ;
      notification_message_ptr_t notification_message_ptr = dynamic_pointer_cast<notification_message>( message_factory( json ) );
      string notification_type = notification_message_ptr->notification_type() ;
      if( notification_type == "worker_broadcast" ) {
	worker_broadcast_message_ptr_t broadcast_ptr = dynamic_pointer_cast<worker_broadcast_message>( notification_message_ptr);
	string notification = broadcast_ptr->notification();
	// create subscriptions for in edges
	if( notification == notification::CREATE_BINDINGS ) {
	  DLOG(INFO) << "creating bindings";
	  create_bindings( pollitems, count_items, size_items );
	}
      } else if( notification_type == notification::JOB_INITIALIZATION ) {
	check_and_start_job( notification_message_ptr );
      } else {
	LOG(WARNING) << "in edge manager get a message that it doesn't understand - " << json ;
      }
    }

  }
  
  void in_edge_manager::check_and_start_job( notification_message_ptr_t message ) {
    DLOG(INFO) << "Got job initialization message ";
    job_initialization_message_ptr_t job_init_message = dynamic_pointer_cast<job_initialization_message>( message );
    CHECK( job_init_message != NULL ) << "invalid notification message cast to job init message";
    // only targeted worker will execute init callback, if such callback has been implemented
    if( job_init_message->worker_type() == worker_type_ ) {
      if( job_init_message->worker_id() == worker_id_ ) {
	initialization_callback cb = context_.get_initialization_notification_callback();
	CHECK( cb != NULL ) << "Sent a job initialization message to a worker without an initialization callback";
	cb();
      }
    }
  }


  bool in_edge_manager::all_messages_arrived( const string& job_id, collaboration_context_t& collab_context ) const {
    return collab_context.job_messages[job_id].size() >= (collab_context.count_items - 1);
    
  }

  void in_edge_manager::handle_collaboration_message( collaboration_context_t& context ) {
    for( int item = 1; item < context.count_items; ++item ) {
      if( ( context.pollitems[item].revents | ZMQ_POLLIN ) == ZMQ_POLLIN ) {
	string json ;
	util::recv( context.pollitems[item].socket, json );
	LOG(INFO) << "Got collaboration message -> " << json ;
        collaboration_message_ptr_t collab_message = dynamic_pointer_cast<collaboration_message>( message_factory( json ) );

	if( collab_message == NULL ) {
	  LOG(WARNING) << "don't know how to handle collaboration message " << json;
	  return;
	}
	
	string collaboration_type = collab_message->collaboration_type();

	if( collaboration_type == "generic" ) {
	  basic_collaboration_message_ptr_t basic_collaboration = dynamic_pointer_cast<basic_collaboration_message>( collab_message) ;
	  string job_id = basic_collaboration->job_id() ;
	  string worker_type = basic_collaboration->worker_type();
	  context.job_messages[job_id][worker_type] =  basic_collaboration->payload() ;
	  if( all_messages_arrived( job_id, context ) ) {
	    collaboration_callback cbfn = context_.get_inedge_message_handler();
	    if( cbfn ) {
	      // hang on to job id so we can attach it to send messages
	      context_.set_job_id( job_id );	      
	      cbfn( context.get_job_messages( job_id ) );
	    } else {
	      LOG(WARNING) << "Got callaboration messages, no callback defined to handle messages";
	    }
	    context.job_messages.erase( job_id );
	  }
	} else {
	  LOG(WARNING) << "don't know how to handle collaboration type " << collaboration_type;
	}
      }
    }
  }

  void in_edge_manager::operator()() {
    DLOG(INFO) << "in edge manager thread started" ;
    zmq_pollitem_t* pollitems = NULL;
    int count_items = 0;
    int size_items = 0;

    try {
      // TODO: fix this if broadcast subscriber is created before broadcast publisher
      //       an exception errno 111 is raised, thus this sleep hack
      //       we want to give the publisher time to be instantiated
      sleep( 5 );
      sub broadcast_subscriber( context_.zmq_context(), HEARTBEAT_RECEIVER_BROADCASTS );
      boost::thread*  edge_monitor_thread = NULL;
      zmq_pollitem_t broadcast_pollitem = {
	broadcast_subscriber.socket(),
	0,
	ZMQ_POLLIN,
	0
      };

      pollitems = (zmq_pollitem_t*)malloc( sizeof( zmq_pollitem_t ) );
      count_items = size_items = 1;
      pollitems[0].socket = broadcast_subscriber.socket();
      pollitems[0].fd = 0;
      pollitems[0].events = ZMQ_POLLIN;
      pollitems[0].revents = 0;

      collaboration_context_t collab_context;
      

      while( true ) {
	int rc = zmq_poll( pollitems, count_items, -1 ); 
	if( rc > 0 ) {
	  handle_notification_message( &pollitems, count_items, size_items );
	  collab_context.count_items = count_items;
	  collab_context.pollitems = pollitems;
	  handle_collaboration_message( collab_context );
	}

      }
    } catch( const util::queue_interrupt& ) {
      DLOG(INFO) << "interrupt terminated in edge manager" ;
    } catch( const std::exception& ex ) { 
      LOG(ERROR) << "exception nuked in edge manager - " << ex.what() ;

    }
    
    

  }

  

}
