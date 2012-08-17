#include "heartbeat.hpp"


namespace kibitz {

  message_ptr notification_message_factory( const ptree& tree ) {
    message_ptr result;
    
    const string notification_type = tree.get<string>( "notification_type" );
    if( notification_type == "heartbeat" ) {
      result = message_ptr( new heartbeat(  tree ) );
    }

    return result;
  }

  message_ptr collaboration_message_factory( const ptree& tree ) {
    message_ptr result;
    return result;
  }

  message_ptr message_factory( const string& json ) {
    message_ptr result;
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

