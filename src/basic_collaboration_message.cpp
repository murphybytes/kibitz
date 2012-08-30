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

}
