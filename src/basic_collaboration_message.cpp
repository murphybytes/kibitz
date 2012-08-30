#include "basic_collaboration_message.hpp"


namespace kibitz {


  string basic_collaboration_message::to_json() const {
    ptree tree;
    stringstream stm;
    collaboration_message::populate_header( tree );
    tree.put( "payload", payload_ );
    tree.put( "worker_type", worker_type_ ); 
    boost::property_tree::json_parser::write_json( stm, tree );
    return  stm.str() ;
  }

  basic_collaboration_message::basic_collaboration_message( const string& worker_type, const string& payload ) 
      :collaboration_message( "generic" ),
       worker_type_( worker_type ),
       payload_(payload) {
    }

  basic_collaboration_message::basic_collaboration_message( const ptree& json )
    :collaboration_message( json ),
     worker_type_( json.get<string>("worker_type") ),
     payload_( json.get<string>( "payload" ) ) {
  }

}
