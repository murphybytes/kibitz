#include "worker_notification_message.hpp"


namespace kibitz {

  worker_notification_message::worker_notification_message( const ptree& json )  
    :notification_message( "worker_query" ),
     worker_type_( json.get<string>("worker_type")),
     worker_id_( json.get<int32_t>("worker_id")),
     host_name_( json.get<string>("host_name")),
     port_( json.get<int16_t>("port") ) {
  }

  worker_notification_message::worker_notification_message( const heartbeat& hb ) 
    :notification_message( "worker_query"), 
     worker_type_( hb.worker_type_ ), 
     worker_id_( hb.worker_id_ ),
     host_name_(hb.host_name_ ),
     port_( hb.port_ ) {
  }

  worker_notification_message::worker_notification_message( const string& worker_type ) 
    :notification_message( "worker_query"),
     worker_type_(worker_type),
     worker_id_( WORKER_ID_UNASSIGNED ),
     port_( PORT_UNASSIGNED ) {
  }

  const string& worker_notification_message::worker_type() const  {
    return worker_type_;
  }

  const string& worker_notification_message::host_name() const {
    return host_name_;
  }

  int32_t worker_notification_message::worker_id() const {
    return worker_id_;
  }

  int16_t worker_notification_message::port() const  {
    return port_;
  }

  string worker_notification_message::to_json() const {
    stringstream stm;
    ptree tree;
    notification_message::populate_header( tree );
    tree.put( "worker_type", worker_type_ );
    tree.put( "worker_id", worker_id_ );
    tree.put( "host_name", host_name_ );
    tree.put( "port", port_ );
    boost::property_tree::json_parser::write_json( stm, tree );
    stm.str();
  }
    


}
