#include "worker_broadcast_message.hpp"

namespace kibitz {
  worker_broadcast_message::worker_broadcast_message( const ptree& json ) 
    :notification_message( "worker_broadcast" ),
     notification_( json.get<string>( "notification" ) ){
  }

  worker_broadcast_message::worker_broadcast_message( const string& notification ) 
    :notification_message( "worker_broadcast" ),
     notification_(notification) {
  }

  string worker_broadcast_message::to_json() const {
    stringstream stm;
    ptree tree;
    notification_message::populate_header( tree );
    tree.put( "notification", notification_ );
    boost::property_tree::json_parser::write_json( stm, tree );
    return stm.str() ;
  }

}
