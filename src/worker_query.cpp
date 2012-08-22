#include "worker_query.hpp"


namespace kibitz {

  worker_query::worker_query( const string& worker_type ) 
    :notification_message( "worker_query" ),
     worker_type_( worker_type ) {
  }

  worker_query::worker_query( const ptree& json ) 
    :notification_message( "worker_query" ),
     worker_type_( json.get<string>("worker_type")) {
  }

  worker_query::~worker_query() {
  }

  string worker_query::to_json() const {
    stringstream stm;
    ptree tree;
    notification_message::populate_header( tree );
    tree.put( "worker_type", worker_type_ );
    boost::property_tree::json_parser::write_json( stm, tree );
    return stm.str();
  }

}
