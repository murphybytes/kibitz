#include "heartbeat.hpp"
#include "inproc_notification_message.hpp"
#include "worker_notification_message.hpp"
#include "worker_query_response.hpp"
#include "worker_query.hpp"
#include "worker_broadcast_message.hpp"
#include "job_initialization_message.hpp"

#include "basic_collaboration_message.hpp"

namespace kibitz {

  message_ptr_t notification_message_factory( const ptree& tree ) {
    message_ptr_t result;
    
    const string notification_type = tree.get<string>( "notification_type" );
    if( notification_type == "heartbeat" ) {
      result = message_ptr_t( new heartbeat(  tree ) );
    }

    if( notification_type == "inproc" ) {
      result = message_ptr_t( new inproc_notification_message( tree ) );
    }
    
    if( notification_type == "worker_broadcast" ) {
      result = message_ptr_t( new worker_broadcast_message( tree ) );
    }

    if( notification_type == "worker_notification" ) {
      result = message_ptr_t( new worker_notification_message( tree ) );
    }

    if( notification_type == "worker_query_response" ) {
      result = message_ptr_t( new worker_query_response( tree ) );
    }

    if( notification_type == "worker_query" ) {
      result = message_ptr_t( new worker_query( tree ) );
    }

    if( notification_type == notification::JOB_INITIALIZATION ) {
      result = message_ptr_t( new job_initialization_message( tree ) );
    }

    return result;
  }

  message_ptr_t collaboration_message_factory( const ptree& tree ) {
    message_ptr_t result;

    const string collaboration_type = tree.get<string>( "collaboration_type" );
    if( collaboration_type == "generic" ) {
      result =  message_ptr_t( new basic_collaboration_message( tree ) );
    }

    return result;
  }

  message_ptr_t message_factory( const string& json ) {
    message_ptr_t result;
    stringstream sstm;
    sstm << json;
    ptree tree;
    boost::property_tree::json_parser::read_json( sstm, tree );
    const string message_type = tree.get<string>( "message_type" );

    if( message_type == "notification" ) {
      result = notification_message_factory( tree ); 
    }

    if( message_type == "collaboration" ) {
      result = collaboration_message_factory( tree );
    }

    return result;
  }

}

